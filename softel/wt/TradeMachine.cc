/*
 * TradeMachine.cc
 *
 *  Created on: Mar 7, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include "TradeMachine.h"

using namespace std;
using namespace SF::CORE::LOCK;
using namespace SF::TRADE;
using namespace SF::TRADE::THRIFT;


namespace SF {
namespace WT {

std::shared_ptr<TradeMachine> Machines::m;
SF::TRADE::THRIFT::TMSpec Machines::tm;

TradeMachine::TradeMachine(const SF::TRADE::THRIFT::TMSpec& tm)
: BaseMachine(tm)
{
    isDemoStarted_ = false;

    init();
}

TradeMachine::~TradeMachine() {
    // TODO Auto-generated destructor stub
    tg_.cancel();
    tg_.wait();
}

void TradeMachine::postOrder(Order& order) {

    DLOGF << TRADE::toString(order) << std::endl;

    try {
        Synchronized l(mxThriftClient_);
        client_->postOrder(order);
    } catch (apache::thrift::TException& e) {
        DLOGF << "TException: " << e.what() << std::endl;
    }
}

void TradeMachine::getOrderBook(SF::TRADE::THRIFT::OrderBook& result) const {
    try {
        Synchronized l(mxThriftClient_);
        client_->getOrderBook(result);
    } catch (apache::thrift::TException& e) {
        DLOGF << "TException: " << e.what() << std::endl;
    }
}

void TradeMachine::getAvgMarketData(SF::TRADE::THRIFT::TMarketDataPointMap& result) const {
    try {
        Synchronized l(mxThriftClient_);
        client_->getMarketData(result);
    } catch (apache::thrift::TException& e) {
        DLOGF << "TException: " << e.what() << std::endl;
    }

}

void TradeMachine::getSymbolsSrc(TSymbols& result) const {
    try {
        Synchronized l(mxThriftClient_);
        client_->getSymbolsSrc(result);
    } catch (apache::thrift::TException& e) {
        DLOGF << "TException: " << e.what() << std::endl;
    }
}
Symbol TradeMachine::getSymbolSrc(size_t idx) const {
    Symbol result;
    try {
        Synchronized l(mxThriftClient_);
        client_->getSymbolSrc(result, idx);
    } catch (apache::thrift::TException& e) {
        DLOGF << "TException: " << e.what() << std::endl;
    }
    return result;
}
void TradeMachine::getSymbolsDst(TSymbols& result) const {
    try {
        Synchronized l(mxThriftClient_);
        client_->getSymbolsDst(result);
    } catch (apache::thrift::TException& e) {
        DLOGF << "TException: " << e.what() << std::endl;
    }
}
Symbol TradeMachine::getSymbolDst(size_t idx) const {
    Symbol result;
    try {
        Synchronized l(mxThriftClient_);
        client_->getSymbolDst(result, idx);
    } catch (apache::thrift::TException& e) {
        DLOGF << "TException: " << e.what() << std::endl;
    }
    return result;
}

void TradeMachine::clear() {
    try {
        Synchronized l(mxThriftClient_);
        client_->clearBook();
    } catch (apache::thrift::TException& e) {
        DLOGF << "TException: " << e.what() << std::endl;
    }
}

void TradeMachine::setSymbols(const SF::TRADE::THRIFT::TSymbols& src, const SF::TRADE::THRIFT::TSymbols& dst) {
    try {
        Synchronized l(mxThriftClient_);
        client_->setSymbols(src, dst);
    } catch (apache::thrift::TException& e) {
        DLOGF << "TException: " << e.what() << std::endl;
    }
}
void TradeMachine::setSymbolsSrc(const SF::TRADE::THRIFT::TSymbols& src) {
    try {
        Synchronized l(mxThriftClient_);
        client_->setSymbolsSrc(src);
    } catch (apache::thrift::TException& e) {
        DLOGF << "TException: " << e.what() << std::endl;
    }
}
void TradeMachine::setSymbolsDst(const SF::TRADE::THRIFT::TSymbols& dst) {
    try {
        Synchronized l(mxThriftClient_);
        client_->setSymbolsDst(dst);
    } catch (apache::thrift::TException& e) {
        DLOGF << "TException: " << e.what() << std::endl;
    }
}

// TODO: Initialize from file/mongo/etc...
void TradeMachine::init() {

    TSymbols src = { newSymbol("Green Apple", "ton", "1 class green apple", "img/green_apple.jpg"),
            newSymbol("Yellow Apple", "ton", "1 class yellow apple", "img/yellow_apple.jpg"),
            newSymbol("Red Apple", "ton", "1 class red apple", "img/red_apple.jpg")
    };

    TSymbols dst = { newSymbol("XBT"), newSymbol("USD"), newSymbol("EUR") };

    setSymbols(src, dst);

    TUsers users = { TRADE::user("User_1"), TRADE::user("User_2"), TRADE::user("User_3"), TRADE::user("User_4"), TRADE::user("User_5") };
    setUsers(users);
}

void TradeMachine::startDemo() {

    WriteSynchronized lock(rw_);

    DLOGF << "isDemoStarted_ = " << isDemoStarted_ << std::endl;

    if (isDemoStarted_) {
        return;
    }

    DLOGF << "isDemoStarted_ = " << isDemoStarted_ << std::endl;

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

            DLOGF << "isDemoStarted_ = " << isDemoStarted_ << " users.size = " << users.size() << std::endl;

            for (int i = 0; i < 100 && !tbb::task::self().is_cancelled(); i++) {
                int u = rand() % users.size();
                Order order;
                order.symbol = symbolsSrc[rand() % symbolsSrc.size()];
                order.symbolDst = symbolsDst[0];
                order.user = users[u];
                TRADE::Price::set(order.price, 1.3 + (double)(rand() % 1000) / 2000);
                TRADE::Volume::set(order.volume, 1 + (double)(rand() % 100) / 20);
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
