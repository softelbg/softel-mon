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
#ifndef SOFTEL_WT_TALLENTMACHINE_H_
#define SOFTEL_WT_TALLENTMACHINE_H_

#include "TradeMachine.h"

namespace SF {
namespace WT {

enum TallentUserType {
    SoftDev,
    SoftQA,
    DevOps
};

class TallentMachine : public TradeMachine {
public:
    explicit TallentMachine(const SF::TRADE::THRIFT::TMSpec& tm);
    virtual ~TallentMachine();

    void init();
    void startDemo();
};


} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_TALLENTMACHINE_H_ */
