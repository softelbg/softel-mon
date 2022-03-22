/*
 * BaseTradeWidget.h
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_BASETRADEWIDGET_H_
#define SOFTEL_WT_BASETRADEWIDGET_H_

#include <Wt/WContainerWidget>
#include "BaseOrderWidget.h"
#include "TradeMachine.h"

namespace SF {
namespace WT {


class BaseTradeWidget : public Wt::WContainerWidget, public UpdateSubscriber {
public:
    explicit BaseTradeWidget(Wt::WContainerWidget *parent = 0);
    virtual ~BaseTradeWidget();

    std::shared_ptr<TradeMachine> getTradeMachine() {
        return tradeMachine_;
    }

protected:
    BaseOrderWidget* orders_;
    Wt::WTable* tableTrade_;
    Wt::WTable* tableTradeHistory_;

    std::shared_ptr<TradeMachine> tradeMachine_;
    std::function<void()> fnUpdater_;
};


} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_BASETRADEWIDGET_H_ */
