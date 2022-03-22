/*
 * OrderBook.h
 *
 *  Created on: Mar 28, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_TRADE_ORDERBOOK_H_
#define SOFTEL_TRADE_ORDERBOOK_H_

#include <softel/core/CObject.h>
#include <softel/core/common.h>
#include <softel/trade/CommonHelpers.h>

namespace SF {
namespace TRADE {

class OrderBook : public SF::CORE::CObject {
public:
    explicit OrderBook(THRIFT::TradeMachine& tm);
    virtual ~OrderBook();

    virtual void postOrder(THRIFT::Order& order);

    const THRIFT::OrderBook& getOrderBook() const {
        return book_;
    }

    void clear();

    std::string toString();

protected:
    virtual void matchOrders(const THRIFT::Order& order);
    void addTransaction(const THRIFT::Order& buy, const THRIFT::Order& sell);

    void orderOpen(const THRIFT::Order& order);

    THRIFT::TradeMachine& tm_;
    THRIFT::OrderBook& book_;
};


} /* namespace TRADE */
} /* namespace SF */

#endif /* SOFTEL_TRADE_ORDERBOOK_H_ */
