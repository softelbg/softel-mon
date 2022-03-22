/*
 * TradeMachine.h
 *
 *  Created on: Apr 21, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_TRADE_TRADEMACHINE_H_
#define SOFTEL_TRADE_TRADEMACHINE_H_

#include <softel/trade/OrderBook.h>

#include <softel/core/common.h>
#include <softel/core/commonDefs.h>
#include <softel/core/util/Options.h>
#include <softel/core/util/OptionParam.hpp>

#include <softel/core/net/zmq/ZMQPublish.h>
#include <softel/core/net/zmq/CGuardZMQMsg.hpp>
#include <softel/trade/idl/messages.pb.h>

#include <softel/trade/idl/gen-cpp/BaseMachine.h>

namespace SF {
namespace TRADE {

class TradeMachine : virtual public THRIFT::BaseMachineIf {
public:
    explicit TradeMachine(const CORE::UTIL::Options& opt);
    virtual ~TradeMachine() noexcept (true);

    void getTM(THRIFT::TradeMachine& _return);

    void getUsers(std::vector<THRIFT::User> & _return);
    void setUsers(const std::vector<THRIFT::User> & users);
    void addUser(const THRIFT::User& user);
    void getUser(THRIFT::User& _return, const int32_t idx);

    void setSymbols(const std::vector<THRIFT::Symbol> & src, const std::vector<THRIFT::Symbol> & dst);
    void setSymbolsSrc(const std::vector<THRIFT::Symbol> & src);
    void setSymbolsDst(const std::vector<THRIFT::Symbol> & dst);
    void getSymbolsSrc(std::vector<THRIFT::Symbol> & _return);
    void getSymbolsDst(std::vector<THRIFT::Symbol> & _return);
    void getSymbolSrc(THRIFT::Symbol& _return, const int32_t idx);
    void getSymbolDst(THRIFT::Symbol& _return, const int32_t idx);

    void clearBook();
    int64_t postOrder(const THRIFT::Order& o);
    int64_t postOrderWithMatch(const THRIFT::Order& o, const std::string& match);
    int64_t matchOrders(const THRIFT::ID id1, const THRIFT::ID id2);
    void getOrderBook(THRIFT::OrderBook& _return);
    void getMarketData(THRIFT::TMarketDataPointMap& _return);

    void waitForUpdates(std::vector<std::string> & _return);

    static void start(const CORE::UTIL::Options& opt);

protected:
    void init();
    void update(const std::string& component);

    CORE::UTIL::Options opt_;

    THRIFT::TradeMachine tm_;
    TRADE::OrderBook book_; // TODO: Move any OrderBook methods as external, currently it wraps THRIFT::OrderBook

    mutable std::shared_ptr<SF::CORE::ZMQPublish> zmqPub_;

    mutable SF::CORE::LOCK::RWMutex rwUser_;
    mutable SF::CORE::LOCK::RWMutex rwSymbol_;
    mutable SF::CORE::LOCK::RWMutex rwOrder_;

    tbb::concurrent_vector<std::string> updates_;
    mutable SF::CORE::LOCK::Mutex rwUpdates_;
    mutable SF::CORE::LOCK::Condition cvUpdates_;
};


} /* namespace TRADE */
} /* namespace SF */

#endif /* SOFTEL_TRADE_TRADEMACHINE_H_ */
