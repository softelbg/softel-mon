/*
 * TradeMachine.cc
 *
 *  Created on: Apr 21, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include <softel/trade/TradeMachine.h>

using namespace std;
using namespace SF::CORE;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::LOCK;

namespace SF {
namespace TRADE {

TradeMachine::TradeMachine(const Options& opt)
: opt_(opt), book_(tm_) {

    deserialize(opt, tm_.spec);
    DLOGF << "TMSpec: " << toString(tm_.spec) << std::endl;

    zmqPub_.reset(new SF::CORE::ZMQPublish("tcp://*:" + std::to_string(zmqPort(tm_.spec.port)), 1000));

    init();
}

TradeMachine::~TradeMachine() noexcept (true) {
    // TODO Auto-generated destructor stub
}

void TradeMachine::init() {

}

void TradeMachine::update(const std::string& component) {
    // ZMQ Pub update
    SF::TRADE::UpdateMessageProto msg;
    msg.set_component(component);
    size_t msgSize = msg.ByteSize();
    CGuardZMQMsg zmqMsg(zmqPub_->getHelper().get(), msgSize);
    msg.SerializeToArray(zmqMsg.Data(), msgSize);
    zmqPub_->Send(zmqMsg);

    // Using internal wait for updates
    //updates_.clear();
    //updates_.push_back(component);
    //cvUpdates_.notify_all();
}

void TradeMachine::start(const Options& opt) {

    boost::shared_ptr<THRIFT::BaseMachineIf> handler(new TradeMachine(opt));

    int port = thriftPort(dynamic_pointer_cast<TradeMachine>(handler)->tm_.spec.port);
    DLOGF << "Port = " << port << std::endl;

    thriftServeNonBlocking(handler, port);
}

void TradeMachine::getTM(THRIFT::TradeMachine& _return) {
    // TODO: Currently no deadlock threat, but have it in mind...
    ReadSynchronized l1(rwUser_);
    ReadSynchronized l2(rwSymbol_);
    ReadSynchronized l3(rwOrder_);
    _return = tm_;
}

void TradeMachine::getUsers(std::vector<THRIFT::User> & _return) {
    ReadSynchronized lock(rwUser_);
    _return = tm_.users;
}

void TradeMachine::setUsers(const std::vector<THRIFT::User>& users) {
    {
        WriteSynchronized lock(rwUser_);
        tm_.users = users;
        
        // Set user id TODO: move to helper method
        /*
        for (size_t i = 0; i < tm_.users.size(); i++) {
            tm_.users[i].id = i;
        }
         */
    }

    update("users");
}

void TradeMachine::addUser(const THRIFT::User& user)
{
    {
        WriteSynchronized lock(rwUser_);
                        
        tm_.users.push_back(user);
        // Set user id TODO: move to helper method
        tm_.users.back().id = tm_.users.size() - 1;
    }

    update("users");
}

void TradeMachine::getUser(THRIFT::User& _return, const int32_t idx) {
    ReadSynchronized lock(rwUser_);
    if (idx >= 0 && idx < tm_.users.size()) {
        _return = tm_.users[idx];
    }
}

void TradeMachine::setSymbols(const std::vector<THRIFT::Symbol>& src, const std::vector<THRIFT::Symbol>& dst) {
    {
        WriteSynchronized lock(rwSymbol_);
        tm_.symbolsSrc = src;
        tm_.symbolsDst = dst;
    }

    update("symbols");
}

void TradeMachine::setSymbolsSrc(const std::vector<THRIFT::Symbol> & src) {
    {
        WriteSynchronized lock(rwSymbol_);
        tm_.symbolsSrc = src;
    }

    update("symbols");
}

void TradeMachine::setSymbolsDst(const std::vector<THRIFT::Symbol> & dst) {
    {
        WriteSynchronized lock(rwSymbol_);
        tm_.symbolsDst = dst;
    }

    update("symbols");
}

void TradeMachine::getSymbolsSrc(std::vector<THRIFT::Symbol>& _return) {
    ReadSynchronized lock(rwSymbol_);
    _return = tm_.symbolsSrc;
}

void TradeMachine::getSymbolsDst(std::vector<THRIFT::Symbol>& _return) {
    ReadSynchronized lock(rwSymbol_);
    _return = tm_.symbolsDst;
}

void TradeMachine::getSymbolSrc(THRIFT::Symbol& _return, const int32_t idx) {
    ReadSynchronized lock(rwSymbol_);
    if (idx >= 0 && idx < tm_.symbolsSrc.size()) {
        _return = tm_.symbolsSrc[idx];
    }
}

void TradeMachine::getSymbolDst(THRIFT::Symbol& _return, const int32_t idx) {
    ReadSynchronized lock(rwSymbol_);
    if (idx >= 0 && idx < tm_.symbolsDst.size()) {
        _return = tm_.symbolsDst[idx];
    }
}

void TradeMachine::clearBook() {
    {
        WriteSynchronized lock(rwOrder_);
        book_.clear();
    }

    update("book");
}

int64_t TradeMachine::postOrder(const THRIFT::Order& o) {
    THRIFT::Order o2 = o;
    {
        WriteSynchronized lock(rwOrder_);
        book_.postOrder(o2);
    }

    update("book");

    return o2.id;
}

int64_t TradeMachine::postOrderWithMatch(const THRIFT::Order& o, const std::string& match) {
    return 0;
}

int64_t TradeMachine::matchOrders(const THRIFT::ID id1, const THRIFT::ID id2) {
    return 0;
}

void TradeMachine::getOrderBook(THRIFT::OrderBook& _return) {
    _return = tm_.book;
}

void TradeMachine::getMarketData(THRIFT::TMarketDataPointMap& _return) {
    _return = tm_.book.md;
}

void TradeMachine::waitForUpdates(std::vector<std::string> & _return) {
    /*
    Synchronized lock(rwUpdates_);
    if (cvUpdates_.wait_for(lock, std::chrono::seconds(10)) == std::cv_status::timeout) {
        _return.push_back("timeout");
    } else {
        //_return.assign(updates_.begin(), updates_.end());
        _return.push_back("all");
    }
    */
}


} /* namespace TRADE */
} /* namespace SF */

