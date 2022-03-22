/*
 * TallentTradeWidget.cc
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include "TallentTradeWidget.h"
#include "TallentMachine.h"
#include "OrderTallentWidget.h"
#include <softel/core/common.h>
#include "common.h"

namespace SF {
namespace WT {


TallentTradeWidget::TallentTradeWidget(Wt::WContainerWidget *parent)
: BaseTradeWidget(parent)
{
    start();
}

TallentTradeWidget::~TallentTradeWidget() {
    Wt::log("info") << __FUNCTION__ << "::" << __LINE__;
}

void TallentTradeWidget::start()
{
    Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;

    // Enable server push
    Wt::WApplication::instance()->enableUpdates(true);

    tradeMachine_ = Machines::m;

    orders_ = new OrderTallentWidget(tradeMachine_);

    tableTrade_ = new Wt::WTable();
    tableTrade_->setHeaderCount(1);
    tableTrade_->setWidth(Wt::WLength("100%"));

    tableTradeHistory_ = new Wt::WTable();
    tableTradeHistory_->setHeaderCount(1);
    tableTradeHistory_->setWidth(Wt::WLength("100%"));

    Wt::WTemplate *tm = new Wt::WTemplate(Wt::WString::tr("TallentTradeMachine"));
    addWidget(tm);
    tm->bindWidget("order-edit", orders_);
    tm->bindWidget("trade-table", tableTrade_);
    tm->bindWidget("tradeHistory-table", tableTradeHistory_);


    /*
    fnUpdater_ = [&] () {

    };
     */

    tradeMachine_->subscribe(Wt::WApplication::instance()->sessionId(), this, fnUpdater_);
    fnUpdater_();
}


} /* namespace WT */
} /* namespace SF */
