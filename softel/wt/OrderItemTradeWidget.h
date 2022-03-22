/*
 * OrderItemTradeWidget.h
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_ORDERITEMTRADEWIDGET_H_
#define SOFTEL_WT_ORDERITEMTRADEWIDGET_H_

#include "BaseOrderWidget.h"

namespace SF {
namespace WT {

class OrderItemTradeWidget : public BaseOrderWidget {
public:
    OrderItemTradeWidget(std::shared_ptr<TradeMachine> tradeMachine, Wt::WContainerWidget *parent = 0);
    virtual ~OrderItemTradeWidget();

private:
    void init();
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_ORDERITEMTRADEWIDGET_H_ */
