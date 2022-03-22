/*
 * BitcoinMachine.cc
 *
 *  Created on: Apr 12, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include <softel/wt/BitcoinMachine.h>
#include <random>
#include <chrono>

using namespace std;
using namespace SF::CORE::LOCK;
using namespace SF::TRADE;


namespace SF {
namespace WT {


BitcoinMachine::BitcoinMachine(const SF::TRADE::THRIFT::TMSpec& tm)
: TradeMachine(tm)
{
    isDemoStarted_ = false;

    init();
}

BitcoinMachine::~BitcoinMachine() {
    // TODO Auto-generated destructor stub
}

void BitcoinMachine::init() {

    DLOGF << std::endl;
    
    THRIFT::TSymbols src = { newSymbol("XBT"), newSymbol("USD"), newSymbol("EUR") };
    THRIFT::TSymbols dst = { newSymbol("XBT"), newSymbol("USD"), newSymbol("EUR") };
       
    THRIFT::Wallet wBtc;
    wBtc.name = "primary Bitcoin";
    wBtc.currency = newSymbol("XBT");
    THRIFT::Wallet wEur;
    wEur.name = "primary EUR";
    wEur.currency = newSymbol("EUR");
    THRIFT::Wallet wUsd;
    wUsd.name = "primary USD";
    wUsd.currency = newSymbol("USD");
    
    THRIFT::User user1 = TRADE::user("Stanislav");
    Volume::set(wBtc.volume, 100.10012345);
    Volume::set(wEur.volume, 10000.22);
    Volume::set(wUsd.volume, 10000);
    user1.wallets[wBtc.currency] = wBtc;   
    user1.wallets[wEur.currency] = wEur;
    user1.wallets[wUsd.currency] = wUsd;
      
    THRIFT::User user2 = TRADE::user("Georgi");
    Volume::set(wBtc.volume, 100.10012345);
    Volume::set(wEur.volume, 10000);
    Volume::set(wUsd.volume, 10000);
    user2.wallets[wBtc.currency] = wBtc;   
    user2.wallets[wEur.currency] = wEur;
    user2.wallets[wUsd.currency] = wUsd;
    
    THRIFT::User user3 = TRADE::user("Nikolay");
    Volume::set(wBtc.volume, 100);
    Volume::set(wEur.volume, 10000);
    Volume::set(wUsd.volume, 10000);
    user3.wallets[wBtc.currency] = wBtc;
    user3.wallets[wEur.currency] = wEur;
    user3.wallets[wUsd.currency] = wUsd;
    
    THRIFT::TUsers users;
    users.push_back(user1);
    users.push_back(user2);
    users.push_back(user3);
    
    setUsers(users);

    setSymbols(src, dst);
}

double BitcoinMachine::getPrice(THRIFT::Symbol src, THRIFT::Symbol dst)
{
    if (src.name == dst.name) {
        return 1;
    }

    std::map<THRIFT::Symbol, std::map<THRIFT::Symbol, double> > prices = {
        { newSymbol("XBT"), { {newSymbol("EUR"), 500}, {newSymbol("USD"), 550} } },
        { newSymbol("EUR"), { {newSymbol("USD"), 1.1} } }
    };

    double price = prices[src][dst];
    
    if (price == 0) {
        price = 1 / getPrice(dst, src);
    }
    
    //DLOG << "src = " << src << std::endl << "dst = " << dst << std::endl << "price = " << price << std::endl;
    
    return price;
}

double BitcoinMachine::getVolume(THRIFT::Symbol src, THRIFT::Symbol dst)
{
    if (src.name == dst.name) {
        return 1;
    }

    std::map<THRIFT::Symbol, std::map<THRIFT::Symbol, double> > volumes = {
        { newSymbol("XBT"), { {newSymbol("EUR"), 1}, {newSymbol("USD"), 1} } },
        { newSymbol("EUR"), { {newSymbol("USD"), 1000}, {newSymbol("XBT"), 500} } },
        { newSymbol("USD"), { {newSymbol("EUR"), 1000}, {newSymbol("XBT"), 550} } },
    };

    double vol = volumes[src][dst];
    
    DLOG << "src = " << src << std::endl << "dst = " << dst << std::endl << "volume = " << vol << std::endl;
    
    return vol;
}

void BitcoinMachine::startDemo() {

    WriteSynchronized lock(rw_);

    DLOGF << "isDemoStarted_ = " << isDemoStarted_ << std::endl;

    if (isDemoStarted_) {
        return;
    }

    DLOGF << "isDemoStarted_ = " << isDemoStarted_ << std::endl;

    isDemoStarted_ = true;

    srand (time(NULL));

    DLOGF << "isDemoStarted_ = " << isDemoStarted_ << std::endl;

    THRIFT::TUsers users;
    getUsers(users);

    THRIFT::TSymbols symbolsSrc;
    getSymbolsSrc(symbolsSrc);
    THRIFT::TSymbols symbolsDst;
    getSymbolsDst(symbolsDst);

    tbb::task_arena limited(4, 1);
    limited.execute([&]{
        tg_.run([=]() {

            DLOGF << "isDemoStarted_ = " << isDemoStarted_ << " users.size = " << users.size() << std::endl;

            for (int i = 0; i < 100 && !tbb::task::self().is_cancelled(); i++) {
                int u = rand() % users.size();
                THRIFT::Order order;
                // XBT has more chance to trade...
                if (rand() % 2 == 0) {
                    order.symbol = symbolsSrc[rand() % symbolsSrc.size()];
                } else {
                    order.symbol = symbolsSrc[0];
                }               
                order.symbolDst = symbolsDst[rand() % symbolsSrc.size()];
                if (order.symbol.name == order.symbolDst.name) {
                    i--;
                    continue;
                }
                order.user = users[u];

                int precision = 1000000;
                double variance = 0.05;
                unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                std::default_random_engine generator(seed);
                std::uniform_int_distribution<int> distribution(- variance * precision, variance * precision);

                TRADE::Price::set(order.price, getPrice(order.symbol, order.symbolDst) * (1 + double(distribution(generator)) / precision));
                TRADE::Volume::set(order.volume, getVolume(order.symbol, order.symbolDst) * (1 + double(distribution(generator)) / precision));
                order.type = u % 2 == 0 ? THRIFT::OrderType::Sell : THRIFT::OrderType::Buy;

                postOrder(order);

                usleep(200000);
                //sleep(1);
            }

            DLOGF << "isDemoStarted_ = " << isDemoStarted_ << std::endl;
            isDemoStarted_ = false;

        });
    });

    DLOGF << "isDemoStarted_ = " << isDemoStarted_ << std::endl;
}


} /* namespace WT */
} /* namespace SF */

