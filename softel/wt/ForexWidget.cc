/*
 * ForexWidget.cc
 *
 *  Created on: Mar 9, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include "ForexWidget.h"

#include <softel/core/commonDefs.h>
#include <softel/core/common.h>
#include "common.h"
#include <softel/core/net/zmq/ZMQSubscription.h>
#include <softel/core/net/zmq/CGuardZMQMsg.hpp>

#include "ForexMachine.h"
#include "OrderForexWidget.h"

using namespace std;
using namespace SF::CORE;
using namespace SF::TRADE;
using namespace SF::TRADE::THRIFT;
using namespace Wt;

namespace SF {
namespace WT {

ForexWidget::ForexWidget(Wt::WContainerWidget *parent)
: BaseTradeWidget(parent)
{
    start();
}

ForexWidget::~ForexWidget()
{
    Wt::log("info") << __FUNCTION__ << "::" << __LINE__;
}

void ForexWidget::start()
{
    Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;

    Wt::WApplication *app = Wt::WApplication::instance();

    // Enable server push
    app->enableUpdates(true);

    TSymbols symbols = { newSymbol("EUR/USD") , newSymbol("XBT/USD"), newSymbol("USD/XBT") };
    Machines::m->setSymbolsSrc(symbols);

    orders_ = new OrderForexWidget(Machines::m);
    OrderForexWidget* orders = dynamic_cast<OrderForexWidget*>(orders_);

    tableTrade_ = new Wt::WTable();
    tableTrade_->setHeaderCount(1);
    tableTrade_->setWidth(Wt::WLength("100%"));

    tableTradeHistory_ = new Wt::WTable();
    tableTradeHistory_->setHeaderCount(1);
    tableTradeHistory_->setWidth(Wt::WLength("100%"));

    tableMarketData_ = new Wt::WTable();
    tableMarketData_->setHeaderCount(1);
    tableMarketData_->setWidth(Wt::WLength("400px"));
    tableMarketData_->elementAt(0, 0)->addWidget(new Wt::WText("Currency"));
    tableMarketData_->elementAt(0, 1)->addWidget(new Wt::WText("BID"));
    tableMarketData_->elementAt(0, 2)->addWidget(new Wt::WText("ASK"));

    Wt::WTemplate *tm = new Wt::WTemplate(Wt::WString::tr("ForexMachine"));
    addWidget(tm);
    tm->bindWidget("marketdata-table", tableMarketData_);
    tm->bindWidget("order-edit", orders_);
    tm->bindWidget("trade-table", tableTrade_);
    tm->bindWidget("tradeHistory-table", tableTradeHistory_);

    // Widget Updates
    std::map<std::string, std::function<void()>> mapComponentUpdates;
    mapComponentUpdates["MarketData"] = [=]() {
        TRADE::TMarketDataPoint md;
        dynamic_cast<ForexMachine*>(Machines::m.get())->getMarketDataLast(md);

        tableMarketData_->clear();

        tableMarketData_->elementAt(0, 0)->addWidget(new Wt::WText("Currency"));
        tableMarketData_->elementAt(0, 1)->addWidget(new Wt::WText("BID"));
        tableMarketData_->elementAt(0, 2)->addWidget(new Wt::WText("ASK"));

        int row = 1;
        for (auto it = md.begin(); it != md.end(); ++it) {
            tableMarketData_->elementAt(row, 0)->addWidget(new Wt::WText(it->second.symbol()));
            tableMarketData_->elementAt(row, 1)->addWidget(new Wt::WText(std::to_string(it->second.bid())));
            tableMarketData_->elementAt(row, 2)->addWidget(new Wt::WText(std::to_string(it->second.ask())));

            tableMarketData_->rowAt(row)->setStyleClass("ks-layouts-Grid-" + std::to_string(row % 2));

            //orders_->setPrice(it->first, (it->second.bid() + it->second.ask()) / 2);

            row++;
        }

        orders->updateMarketData(md);

        Wt::WApplication::instance()->triggerUpdate();
    };

    tradeMachine_ = Machines::m;

    Machines::m->start();

    Machines::m->subscribe(Wt::WApplication::instance()->sessionId(), this, fnUpdater_, mapComponentUpdates);
    fnUpdater_();
}


} /* namespace WT */
} /* namespace SF */
