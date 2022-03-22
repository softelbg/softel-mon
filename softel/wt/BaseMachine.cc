/*
 * BaseMachine.cc
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include "BaseMachine.h"

#include <Wt/WServer>
#include <softel/trade/idl/Protobuf.h>
#include <softel/trade/TradeMachine.h>

using namespace std;
using namespace SF::CORE::LOCK;
using namespace SF::CORE::UTIL;
using namespace SF::CORE;
using namespace SF::TRADE;
using namespace SF::TRADE::THRIFT;

namespace SF {
namespace WT {

BaseMachine::BaseMachine()
: id_(0)
{

}

BaseMachine::BaseMachine(const SF::TRADE::THRIFT::TMSpec& tm)
: id_(0), tm_(tm)
{
    // TODO: Put Host to TMachine to define full host:port url...
    {
        Synchronized l(mxThriftClient_);
        client_ = thriftClientNonBlocking(thriftPort(tm_.port));
    }
    zmqSub_.reset(new SF::CORE::ZMQSubscription("tcp://localhost:" + std::to_string(zmqPort(tm.port))));

    tbb::task_arena limited(4, 1);
    limited.execute([&]{
        tg_.run([=]() {
            while (true) {
                zmqSub_->Receive([&](SF::CORE::CGuardZMQMsg& msg, const int size) {
                    if (size == 0) {
                        return;
                    }

                    ILOG << "size = " << size;
                    ILOG << SF::CORE::Helper::toHex((unsigned char*)msg.Data(), size);

                    UpdateMessageProto msgUpdate;
                    msgUpdate.ParseFromArray(msg.Data(), msg.DataSize());

                    ILOG << "Update: " << msgUpdate.component() << std::endl;

                    update();
                });
            }
        });
    });
}

BaseMachine::~BaseMachine() noexcept (false) {
    // TODO Auto-generated destructor stub
}

void BaseMachine::connectMachine() {

}

void BaseMachine::subscribe(const std::string& sessionId, UpdateSubscriber* client, const std::function<void()>& callback) {
    WriteSynchronized lock(rw_);
    subscriptions_.insert(std::make_pair(client, UpdateSubscription(sessionId, client, callback)));
}

void BaseMachine::subscribe(const std::string& sessionId,
        UpdateSubscriber* client,
        const std::function<void()>& callback,
        const std::map<std::string, std::function<void()>>& components)
{
    WriteSynchronized lock(rw_);
    UpdateSubscription s(sessionId, client, callback);
    s.components_ = components;
    subscriptions_.insert(std::make_pair(client, s));
}

void BaseMachine::unsubscribe(UpdateSubscriber *client) {
    WriteSynchronized lock(rw_);
    DLOGF << "erasing... " << std::endl;
    subscriptions_.erase(client);
    DLOGF << "erased" << std::endl;
}

void BaseMachine::update() {
    ReadSynchronized lock(rw_);
    DLOGF << " subscriptions_ size = " << subscriptions_.size() << " Full Update" << std::endl;
    for (auto it = subscriptions_.begin(); it != subscriptions_.end(); ++it) {
        UpdateSubscription& c = it->second;
        Wt::WServer::instance()->post(c.sessionId, c.function);
    }
}

void BaseMachine::update(const std::string& component) {
    ReadSynchronized lock(rw_);
    DLOGF << " subscriptions_ size = " << subscriptions_.size() << " Update component = " << component << std::endl;
    for (auto it = subscriptions_.begin(); it != subscriptions_.end(); ++it) {
        UpdateSubscription& c = it->second;
        auto itComponent = c.components_.find(component);
        if (itComponent != c.components_.end()) {
            Wt::WServer::instance()->post(c.sessionId, itComponent->second);
        }
    }
}

// TODO: remove this dual behavior
void BaseMachine::getUsers(TUsers& result) const {
    if (client_) {
        try {
            Synchronized l(mxThriftClient_);
            client_->getUsers(result);
        } catch (apache::thrift::TException& e) {
            DLOGF << "TException: " << e.what() << std::endl;
        }
    } else {
        ReadSynchronized lock(rw_);
        result = users_;
    }
}

User BaseMachine::getUser(size_t idx) const {
    User result;
    if (client_) {
        try {
            Synchronized l(mxThriftClient_);
            client_->getUser(result, idx);
        } catch (apache::thrift::TException& e) {
            DLOGF << "TException: " << e.what() << std::endl;
        }
    } else {
        if (idx >= 0 && idx < users_.size()) {
            result = users_[idx];
        }
    }
    return result;
}

void setUsersWalletsDummy(TUsers& users)
{
    THRIFT::Wallet wBtc;
    wBtc.name = "primary Bitcoin";
    wBtc.currency = newSymbol("XBT");
    THRIFT::Wallet wEur;
    wEur.name = "primary EUR";
    wEur.currency = newSymbol("EUR");
    THRIFT::Wallet wUsd;
    wUsd.name = "primary USD";
    wUsd.currency = newSymbol("USD");
    TRADE::Volume::set(wBtc.volume, 1000.10012345);
    TRADE::Volume::set(wEur.volume, 100000.22);
    TRADE::Volume::set(wUsd.volume, 100000);
    
    for (size_t i = 0; i < users.size(); i++) {
        users[i].wallets[wBtc.currency] = wBtc;   
        users[i].wallets[wEur.currency] = wEur;
        users[i].wallets[wUsd.currency] = wUsd;
    }
}

void BaseMachine::setUsers(const TUsers& users) {
    if (client_) {
        try {
            TUsers users_copy = users;
            setUsersWalletsDummy(users_copy);
            Synchronized l(mxThriftClient_);
            client_->setUsers(users_copy);
        } catch (apache::thrift::TException& e) {
            DLOGF << "TException: " << e.what() << std::endl;
        }
    } else {
        WriteSynchronized lock(rw_);
        users_ = users;
    }
}

void BaseMachine::setOwner(const User& owner) {

    WriteSynchronized lock(rw_);
    owner_ = owner;

}
void BaseMachine::getOwner(User& owner) const {
    ReadSynchronized lock(rw_);
    owner = owner_;
}


} /* namespace WT */
} /* namespace SF */
