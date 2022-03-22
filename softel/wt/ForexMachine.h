/*
 * ForexMachine.h
 *
 *  Created on: Mar 9, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_FOREXMACHINE_H_
#define SOFTEL_WT_FOREXMACHINE_H_

#include "TradeMachine.h"

namespace SF {
namespace WT {


class ForexMachine : public TradeMachine {
public:
    explicit ForexMachine(const SF::TRADE::THRIFT::TMSpec& tm);
    virtual ~ForexMachine();

    void getMarketDataLast(SF::TRADE::TMarketDataPoint& result) const;

    void start();

private:
    //virtual bool calcOrder(Order& order);

    SF::TRADE::TMarketDataPoint mdLast_;

    std::shared_ptr<std::thread> workThread_;
    std::atomic<bool> isWorkerStopped_;
    std::atomic<bool> isStarted_;
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_FOREXMACHINE_H_ */
