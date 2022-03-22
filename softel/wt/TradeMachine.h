/*
 * TradeMachine.h
 *
 *  Created on: Mar 7, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_TRADEMACHINE_H_
#define SOFTEL_WT_TRADEMACHINE_H_

#include "BaseMachine.h"
#include <softel/core/common.h>
#include <softel/trade/OrderBook.h>

namespace SF {
namespace WT {


class TradeMachine : public BaseMachine {
public:
    explicit TradeMachine(const SF::TRADE::THRIFT::TMSpec& tm);
    virtual ~TradeMachine() noexcept (false);

    void setSymbols(const SF::TRADE::THRIFT::TSymbols& src, const SF::TRADE::THRIFT::TSymbols& dst);
    void setSymbolsSrc(const SF::TRADE::THRIFT::TSymbols& src);
    void setSymbolsDst(const SF::TRADE::THRIFT::TSymbols& dst);

    // TODO: decide which needs to be synced...
    void getSymbolsSrc(SF::TRADE::THRIFT::TSymbols& result) const;
    SF::TRADE::THRIFT::Symbol getSymbolSrc(size_t idx) const;
    void getSymbolsDst(SF::TRADE::THRIFT::TSymbols& result) const;
    SF::TRADE::THRIFT::Symbol getSymbolDst(size_t idx) const;

    void getOrderBook(SF::TRADE::THRIFT::OrderBook& result) const;
    void getAvgMarketData(SF::TRADE::THRIFT::TMarketDataPointMap& result) const;

    void postOrder(SF::TRADE::THRIFT::Order& order);

    void clear();

    virtual void init();
    virtual void startDemo();

protected:
    std::atomic<bool> isDemoStarted_;
};


// TODO: just to keep the single instances, make it better...
struct Machines {
    static std::shared_ptr<TradeMachine> m;
    static SF::TRADE::THRIFT::TMSpec tm; // Template
};


} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_TRADEMACHINE_H_ */
