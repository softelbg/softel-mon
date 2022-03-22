/*
 * TallentMachine.cc
 *
 *  Created on: Apr 12, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include <softel/wt/TallentMachine.h>

using namespace std;
using namespace SF::CORE::LOCK;
using namespace SF::TRADE;
using namespace SF::TRADE::THRIFT;


namespace SF {
namespace WT {


TallentMachine::TallentMachine(const SF::TRADE::THRIFT::TMSpec& tm)
: TradeMachine(tm)
{
    isDemoStarted_ = false;

    init();
}

TallentMachine::~TallentMachine() {
    // TODO Auto-generated destructor stub
}

void TallentMachine::init() {

    DLOGF << std::endl;

    TUsers users = {
            TRADE::user("Stanislav Georgiev", {{THRIFT::g_trade_constants.USER_TYPE_TALLENT, "SoftDev"}},
                    { { THRIFT::g_trade_constants.USER_SKILLS, { {"c++", "15"}, {"java", "15"}, {"php", "5"}, {"asm", "23"} } },
                            { THRIFT::g_trade_constants.USER_LINKS, { {"in", "https://bg.linkedin.com/in/stanislavgeorgiev"} } }
                    }
            ),
            TRADE::user("Georgi Ivanov", {{THRIFT::g_trade_constants.USER_TYPE_TALLENT, "SoftDev"}},
                    { { THRIFT::g_trade_constants.USER_SKILLS, { {"c++", "15"}, {"linux", "10"} } }, {}
                    }
            ),
            TRADE::user("Nikolay Mitev", {{THRIFT::g_trade_constants.USER_TYPE_TALLENT, "SoftDev"}},
                    { { THRIFT::g_trade_constants.USER_SKILLS, { {"c++", "15"}, {"linux", "10"} } }, {}
                    }
            ),
            TRADE::user("Dev_3", {{THRIFT::g_trade_constants.USER_TYPE_TALLENT, "SoftDev"}},
                    { { THRIFT::g_trade_constants.USER_SKILLS, { {"php", "5"}, {"js", "5"} } }, {}
                    }
            ),
            TRADE::user("DevOps_1", {{THRIFT::g_trade_constants.USER_TYPE_TALLENT, "DevOps"}},
                    { { THRIFT::g_trade_constants.USER_SKILLS, { {"win", "10"}, {"linux", "10"}, {"aws", "5"} } }, {}
                    }
            ),

            TRADE::user("VMware"), TRADE::user("Visteon"), TRADE::user("Questers"), TRADE::user("SAP"), TRADE::user("TallentHunter_5")
    };

    setUsers(users);

    TSymbols src = { newSymbol("c++", "hours per week"), newSymbol("java", "hours per week"),
            newSymbol("php", "hours per week"), newSymbol("js", "hours per week"),
            newSymbol("aws", "hours per week")
    };

    TSymbols dst = { newSymbol("BGN"), newSymbol("EUR"), newSymbol("USD"), newSymbol("XBT") };

    setSymbols(src, dst);
}

void SF::WT::TallentMachine::startDemo() {

    clear();

    DLOGF << "isDemoStarted_ = " << isDemoStarted_ << std::endl;

    WriteSynchronized lock(rw_);

    if (isDemoStarted_) {
        return;
    }

    isDemoStarted_ = true;

    srand (time(NULL));

    DLOGF << "isDemoStarted_ = " << isDemoStarted_ << std::endl;

    TUsers users;
    getUsers(users);

    TSymbols symbolsSrc;
    getSymbolsSrc(symbolsSrc);
    TSymbols symbolsDst;
    getSymbolsDst(symbolsDst);

    tbb::task_arena limited(4, 1);
    limited.execute([&]{
        tg_.run([=]() {

            map<string, int> mapTallentHours;

            for (int i = 0; i < 100 && !tbb::task::self().is_cancelled(); i++) {
                int u = rand() % users.size();

                Order order;
                order.symbolDst = symbolsDst[0];
                order.user = users[u];

                int idxSym = 0;

                double price = 10 + rand() % 20 + 50 / (idxSym + 1);
                double vol = 10 + rand() % 30;
                TRADE::Price::set(order.price, price);
                TRADE::Volume::set(order.volume, vol);

                if (order.user.kv.count(THRIFT::g_trade_constants.USER_TYPE_TALLENT)) {
                    order.type = THRIFT::OrderType::Sell;

                    if (mapTallentHours[order.user.username] + vol > 60) {
                        i--;
                        continue;
                    }

                    mapTallentHours[order.user.username] += vol;

                    auto itSkills = order.user.kvs.find(THRIFT::g_trade_constants.USER_SKILLS);

                    // TODO: rework here...
                    while (true) {
                        idxSym = rand() % symbolsSrc.size();
                        order.symbol = symbolsSrc[idxSym];

                        if (itSkills->second.count(order.symbol.name)) {
                            break;
                        }
                    }
                } else {
                    DLOGF << endl;
                    idxSym = rand() % symbolsSrc.size();
                    DLOGF << "idxSym = " << idxSym << endl;
                    order.symbol = symbolsSrc[idxSym];
                    order.type = THRIFT::OrderType::Buy;
                    DLOGF << endl;
                }

                postOrder(order);

                usleep(200000);
                //sleep(1);
            }

            isDemoStarted_ = false;
        });
    });

    DLOGF << "isDemoStarted_ = " << isDemoStarted_ << std::endl;
}


} /* namespace WT */
} /* namespace SF */

