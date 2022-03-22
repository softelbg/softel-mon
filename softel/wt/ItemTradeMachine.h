/*
 * ItemTradeMachine.h
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_ITEMTRADEMACHINE_H_
#define SOFTEL_WT_ITEMTRADEMACHINE_H_

#include "TradeMachine.h"

namespace SF {
namespace WT {

class ItemTradeMachine : public TradeMachine {
public:
    explicit ItemTradeMachine(const SF::TRADE::THRIFT::TMSpec& tm);
    virtual ~ItemTradeMachine();

private:
    void fillOrderTable(SF::TRADE::THRIFT::TOrders::const_iterator begin, SF::TRADE::THRIFT::TOrders::const_iterator end, int col, int& row);
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_ITEMTRADEMACHINE_H_ */
