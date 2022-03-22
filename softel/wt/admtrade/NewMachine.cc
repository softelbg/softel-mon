/*
 * NewMachine.cc
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include "NewMachine.h"

#include <softel/core/commonDefs.h>
#include <softel/core/common.h>
#include <softel/wt/common.h>

using namespace std;
using namespace SF::CORE::LOCK;
using namespace SF::TRADE;
using namespace SF::TRADE::THRIFT;


namespace SF {
namespace WT {

std::shared_ptr<NewMachine> AdminMachines::admnewmachine(new NewMachine());

NewMachine::NewMachine()
: BaseMachine(), port_(4010), host_("tradexbox.com")
{
    types_ = { TRADING_WIDGET, BITCOIN_TRADE_WIDGET, FOREX_WIDGET, ITEM_TRADE_WIDGET, TALLENT_TRADE_WIDGET };

    initDemo();
}

NewMachine::~NewMachine() {
    // TODO Auto-generated destructor stub
}

void NewMachine::initDemo() {

    users_ = { TRADE::user("Adm_User_1"), TRADE::user("Adm_User_2"),
            TRADE::user("Adm_User_3"), TRADE::user("Adm_User_4"), TRADE::user("Adm_User_5") };

    vector<SF::TRADE::THRIFT::TMSpec> machines = {
            newTMSpec(TRADE::user("Adm_User_1"), BITCOIN_TRADE_WIDGET, "BitEx Sofia"),
            
            newTMSpec(TRADE::user("Adm_User_1"), TRADING_WIDGET, "Agro Trade Haskovo"),
            newTMSpec(TRADE::user("Adm_User_2"), TRADING_WIDGET, "Agro Trade Plovdiv"),
            newTMSpec(TRADE::user("Adm_User_3"), TRADING_WIDGET, "Agro Trade Sofia"),

            newTMSpec(TRADE::user("Adm_User_1"), FOREX_WIDGET, "XBT forex BG"),
            newTMSpec(TRADE::user("Adm_User_4"), FOREX_WIDGET, "XBT forex DE"),
            newTMSpec(TRADE::user("Adm_User_5"), FOREX_WIDGET, "XBT forex US"),

            newTMSpec(TRADE::user("Adm_User_1"), ITEM_TRADE_WIDGET, "Collectible Items Trade Haskovo"),
            newTMSpec(TRADE::user("Adm_User_2"), ITEM_TRADE_WIDGET, "Collectible Items Trade Plovdiv"),
            newTMSpec(TRADE::user("Adm_User_3"), ITEM_TRADE_WIDGET, "Collectible Items Trade Sofia"),

            newTMSpec(TRADE::user("Adm_User_3"), TALLENT_TRADE_WIDGET, "Software Dev Contractors Sofia"),
            newTMSpec(TRADE::user("Adm_User_3"), TALLENT_TRADE_WIDGET, "Software Dev Contractors Romania"),
            newTMSpec(TRADE::user("Adm_User_3"), TALLENT_TRADE_WIDGET, "Software Dev Contractors Global")
    };

    for (auto it = machines.begin(); it != machines.end(); ++it) {
        go(*it);
    }
}

void NewMachine::go(SF::TRADE::THRIFT::TMSpec& m) {
    {
        WriteSynchronized lock(rw_);

        DLOGF << "Go Machine: " << m.type << std::endl;

        m.port = port_;
        m.link = "http://" + host_ + ":" + std::to_string(port_);
        //m.timeCreation = std::chrono::high_resolution_clock::now();
        m.timeCreation = time(0);
        m.guid = makeGuid(m.type + "::" + m.owner.username + "::" + std::to_string(port_));

        m.cmd.push_back("../../../bin/tmcore" + SF::TRADE::serialize(m));
        m.cmd.push_back("../../../bin/wtm --docroot /usr/local/share/Wt --http-address 0.0.0.0 --http-port " + std::to_string(port_) + SF::TRADE::serialize(m));

        ILOGF << "M: " << toString(m)<< std::endl;
        for (auto cmd : m.cmd) {
            std::string cmdExec = "nohup " + cmd + " > /dev/null &";
            ILOGF << "Executing... " << cmdExec << std::endl;
            system(cmdExec.c_str());
            ILOGF << "Executed" << std::endl;
        }

        machines_[m.guid] = m;
        // Added to User index.
        idxUser_[m.owner.username].push_back(m.guid);

        port_++;
    }

    update();
}

void NewMachine::killall() {
    {
        WriteSynchronized lock(rw_);

        system("killall trademachine");

        machines_.clear();
        idxUser_.clear();

        port_ = 4010;
    }

    update();
}

void NewMachine::kill(const SF::TRADE::THRIFT::TMSpec& m) {
    {
        WriteSynchronized lock(rw_);
        DLOGF << "kill Machine: " << m.guid << std::endl;

        for (auto cmd : m.cmd) {
            std::string cmdKill = "pkill -f \"" + cmd + "\"";
            ILOGF << "cmdKill" << cmdKill << std::endl;
            system(cmdKill.c_str());
        }

        machines_.erase(m.guid);
    }

    update();
}

void NewMachine::getMyMachines(TMachines& machines) const {
    getMachines(owner_, machines);
}

void NewMachine::getMachines(TMachines& machines) const {
    ReadSynchronized lock(rw_);
    machines = machines_;
}

void NewMachine::getMachines(const User& user, TMachines& machines) const {
    ReadSynchronized lock(rw_);

    auto itUserMachines = idxUser_.find(user.username);
    if (itUserMachines != idxUser_.end()) {
        for (auto itMachineId = itUserMachines->second.begin(); itMachineId != itUserMachines->second.end(); ++itMachineId) {
            auto itMachine = machines_.find(*itMachineId);
            if (itMachine != machines_.end()) {
                DLOGF << " found " << *itMachineId << std::endl;
                machines[*itMachineId] = itMachine->second;
            }
        }
    }
}

void NewMachine::getMachines(size_t idxOwner, TMachines& machines) const {
    DLOGF << "owner id = " << idxOwner << std::endl;

    User owner;

    {
        ReadSynchronized lock(rw_);

        if (idxOwner >= 0 && idxOwner < users_.size()) {
            owner = users_[idxOwner];
        }
    }

    DLOGF << " user = " << owner.username << std::endl;

    getMachines(owner, machines);
}

void NewMachine::getTypes(TTypes& types) const {
    ReadSynchronized lock(rw_);
    types = types_;
}
std::string NewMachine::getType(size_t idx) const {
    ReadSynchronized lock(rw_);

    std::string result;

    if (idx >= 0 && idx < types_.size()) {
        result = types_[idx];
    }

    return result;
}

} /* namespace WT */
} /* namespace SF */
