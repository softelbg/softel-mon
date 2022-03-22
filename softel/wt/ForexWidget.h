/*
 * ForexWidget.h
 *
 *  Created on: Mar 9, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_FOREXWIDGET_H_
#define SOFTEL_WT_FOREXWIDGET_H_

#include <Wt/WContainerWidget>
#include <softel/core/common.h>

#include "BaseTradeWidget.h"
#include "TradingWidget.h"
#include "ForexMachine.h"
#include "OrderMarketWidget.h"

namespace SF {
namespace WT {


class ForexWidget : public BaseTradeWidget {
public:
    explicit ForexWidget(Wt::WContainerWidget *parent = 0);
    virtual ~ForexWidget();

private:
    void start();

    Wt::WTable *tableMarketData_;
};


} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_FOREXWIDGET_H_ */
