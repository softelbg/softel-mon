/* 
 * File:   BitcoinTradeWidget.cpp
 * Author: stan
 * 
 * Created on October 8, 2016, 6:26 PM
 * 
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 * 
 */

#include "BitcoinTradeWidget.h"

namespace SF {
namespace WT {

using namespace std;
using namespace SF::CORE;
using namespace Wt;

BitcoinTradeWidget::BitcoinTradeWidget(WContainerWidget *parent)
: BaseTradeWidget(parent)
{
    start();
}

BitcoinTradeWidget::~BitcoinTradeWidget() {
    Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;
}

void BitcoinTradeWidget::start()
{
    Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;

    // Enable server push
    Wt::WApplication::instance()->enableUpdates(true);

    orders_ = new OrderBitcoinWidget(Machines::m);

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

OrderBitcoinWidget::OrderBitcoinWidget(std::shared_ptr<TradeMachine> tradeMachine, WContainerWidget *parent)
: OrderMarketWidget(tradeMachine, parent)
{
    spPrice_->setValue(500);
    WComboBox* cbSymbolDst = dynamic_cast<WComboBox*>(cbSymbolDst_);
    tradeMachine_->getSymbolsDst(symbolsDst_);
    updateComboBox(symbolsDst_, cbSymbolDst, false, 2);
}

} /* namespace WT */
} /* namespace SF */