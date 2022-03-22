/*
 * ItemTradeWidget.h
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_ITEMTRADEWIDGET_H_
#define SOFTEL_WT_ITEMTRADEWIDGET_H_

#include <Wt/WContainerWidget>
#include <softel/core/common.h>

#include "BaseTradeWidget.h"
#include "TradingWidget.h"
#include "ItemTradeMachine.h"
#include "OrderItemTradeWidget.h"

namespace SF {
namespace WT {

class ItemTradeWidget : public BaseTradeWidget {
public:
    explicit ItemTradeWidget(Wt::WContainerWidget *parent = 0);
    virtual ~ItemTradeWidget();

private:
    void start();
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_ITEMTRADEWIDGET_H_ */
