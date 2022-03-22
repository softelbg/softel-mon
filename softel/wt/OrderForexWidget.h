/*
 * OrderForexWidget.h
 *
 *  Created on: Mar 16, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_ORDERFOREXWIDGET_H_
#define SOFTEL_WT_ORDERFOREXWIDGET_H_

#include "BaseOrderWidget.h"
#include "ForexMachine.h"

namespace SF {
namespace WT {

class OrderForexWidget : public BaseOrderWidget {
public:
    OrderForexWidget(std::shared_ptr<TradeMachine> tradeMachine, Wt::WContainerWidget *parent = 0);
    virtual ~OrderForexWidget();

    void updateMarketData(const SF::TRADE::TMarketDataPoint& md);

private:
    void init();

    Wt::WComboBox* cbType_;
    Wt::WText* txtBid_;
    Wt::WText* txtAsk_;
    Wt::WDoubleSpinBox* spStopLoss_;
    Wt::WDoubleSpinBox* spTakeProfit_;
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_ORDERFOREXWIDGET_H_ */
