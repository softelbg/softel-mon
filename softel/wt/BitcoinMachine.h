/*
 * TallentMachine.h
 *
 *  Created on: Apr 12, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_BITCOINMACHINE_H_
#define SOFTEL_WT_BITCOINMACHINE_H_

#include "TradeMachine.h"

namespace SF {
namespace WT {


class BitcoinMachine : public TradeMachine {
public:
    explicit BitcoinMachine(const SF::TRADE::THRIFT::TMSpec& tm);
    virtual ~BitcoinMachine();

    void init();
    void startDemo();
    
private:
    double getPrice(SF::TRADE::THRIFT::Symbol src, SF::TRADE::THRIFT::Symbol dst);
    double getVolume(SF::TRADE::THRIFT::Symbol src, SF::TRADE::THRIFT::Symbol dst);
};


} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_BITCOINMACHINE_H_ */
