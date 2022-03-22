/*
 * TradingWidget.cc
 *
 *  Created on: Feb 13, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "TradingWidget.h"
#include <softel/core/commonDefs.h>
#include <softel/core/common.h>
#include "common.h"

namespace SF {
namespace WT {

using namespace std;
using namespace SF::CORE;
using namespace Wt;


TradingWidget::TradingWidget(WContainerWidget *parent)
: BaseTradeWidget(parent)
{
    start();
}

TradingWidget::~TradingWidget() {
    Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;
}

void TradingWidget::start()
{
    Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;

    // Enable server push
    Wt::WApplication::instance()->enableUpdates(true);

    orders_ = new OrderMarketWidget(Machines::m);

    tableTrade_ = new Wt::WTable();
    tableTrade_->setHeaderCount(1);
    tableTrade_->setWidth(Wt::WLength("100%"));

    tableTradeHistory_ = new Wt::WTable();
    tableTradeHistory_->setHeaderCount(1);
    tableTradeHistory_->setWidth(Wt::WLength("100%"));

    Wt::WTemplate *tm = new Wt::WTemplate(Wt::WString::tr("TradeMachine"));
    addWidget(tm);
    tm->bindWidget("order-edit", orders_);
    tm->bindWidget("trade-table", tableTrade_);
    tm->bindWidget("tradeHistory-table", tableTradeHistory_);

    tradeMachine_ = Machines::m;

    Machines::m->subscribe(Wt::WApplication::instance()->sessionId(), this, fnUpdater_);
    fnUpdater_();
}


} /* namespace WT */
} /* namespace SF */
