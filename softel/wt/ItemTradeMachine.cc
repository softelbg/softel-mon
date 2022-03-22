/*
 * ItemTradeMachine.cc
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include "ItemTradeMachine.h"

namespace SF {
namespace WT {

ItemTradeMachine::ItemTradeMachine(const SF::TRADE::THRIFT::TMSpec& tm)
: TradeMachine(tm)
{
    init();

    SF::TRADE::THRIFT::TSymbols src;
    SF::TRADE::THRIFT::TSymbols dst;

    setSymbols(src, dst);
}

ItemTradeMachine::~ItemTradeMachine() {
    // TODO Auto-generated destructor stub
}


} /* namespace WT */
} /* namespace SF */
