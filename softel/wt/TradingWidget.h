/*
 * TradingWidget.h
 *
 *  Created on: Feb 13, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef SOFTEL_WT_TRADINGWIDGET_H_
#define SOFTEL_WT_TRADINGWIDGET_H_

#include <Wt/WContainerWidget>
#include <Wt/WTable>

#include <softel/core/common.h>

#include "BaseTradeWidget.h"
#include "TradeMachine.h"
#include "OrderMarketWidget.h"

namespace SF {
namespace WT {


class TradingWidget : public BaseTradeWidget {
public:
	explicit TradingWidget(Wt::WContainerWidget *parent = 0);
	virtual ~TradingWidget();

private:
	void start();
};


} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_TRADINGWIDGET_H_ */
