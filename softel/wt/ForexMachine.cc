/*
 * ForexMachine.cc
 *
 *  Created on: Mar 9, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include "ForexMachine.h"

#include <softel/core/commonDefs.h>
#include <softel/core/common.h>
#include "common.h"
#include <softel/core/net/zmq/ZMQSubscription.h>
#include <softel/core/net/zmq/CGuardZMQMsg.hpp>

using namespace std;
using namespace SF::CORE::LOCK;
using namespace SF::TRADE;

namespace SF {
namespace WT {

ForexMachine::ForexMachine(const SF::TRADE::THRIFT::TMSpec& tm)
: TradeMachine(tm), isWorkerStopped_(false), isStarted_(false)
{
}

ForexMachine::~ForexMachine() {
    isWorkerStopped_ = true;
    workThread_->join();
    SF::CORE::ZMQBase::destroy();
    ILOGF << " ended";
}

void ForexMachine::start() {

    WriteSynchronized lock(rw_);

    if (isStarted_) {
        return;
    }

    isStarted_ = true;

    workThread_.reset(new std::thread([&]() {
        std::string url = "tcp://localhost:21981";
        ILOGF << " url = " << url << std::endl;
        SF::CORE::ZMQSubscription zmqSub(url);

        while (!isWorkerStopped_) {
            zmqSub.Receive([&](SF::CORE::CGuardZMQMsg& msg, const int size) {

                if (size == 0) {
                    return;
                }

                SF::TRADE::MarketDataProto md;
                md.ParseFromArray(msg.Data(), msg.DataSize());

                //ILOG << md.symbol() << " BID[" << md.bid() << "] ASK[" << md.ask() << "]" << std::endl;

                {
                    WriteSynchronized lock(rw_);
                    mdLast_[newSymbol(md.symbol())] = md;
                }

                update("MarketData");
            });
        }
    }));

}

void ForexMachine::getMarketDataLast(TMarketDataPoint& result) const {
    ReadSynchronized lock(rw_);
    result = mdLast_;
}

/*
bool ForexMachine::calcOrder(Order& order) {

    DLOGF << order.toString() << std::endl;

    bool isValid = true;

    order.symbolDst = order.symbol;

    switch (order.type) {
        case OrderType::Buy :
            order.setPrice(mdLast_[order.symbol.name].ask());
            break;
        case OrderType::Sell :
            order.setPrice(mdLast_[order.symbol.name].bid());
            break;
    }

    for (auto it = orders_.begin(); it != orders_.end(); ++it) {

        Order& o2 = *it;

        if (order.symbol.name == o2.symbol.name) {
            if (order.type == o2.type) {
                if (order.user.username == o2.user.username) {
                    o2.volume += order.volume;
                    isValid = false;
                    break;
                }
            } else {
                if (o2.volume > order.volume) {
                    o2.volume -= order.volume;
                    addTransaction(o2, order);
                    isValid = false;
                    break;
                } else if (o2.volume == order.volume) {
                    addTransaction(o2, order);
                    orders_.erase(it);
                    isValid = false;
                    break;
                } else {
                    order.volume -= o2.volume;
                    addTransaction(order, o2);
                    orders_.erase(it);
                    if (orders_.empty()) {
                        break;
                    }
                    it = orders_.begin();
                    continue;
                }
            }
        }
    }

    return isValid;
}
*/

} /* namespace WT */
} /* namespace SF */
