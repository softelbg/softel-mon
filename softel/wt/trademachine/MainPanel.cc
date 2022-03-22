/*
 * MainPanel.cc
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include "MainPanel.h"

#include <softel/wt/common.h>
#include <softel/wt/TradingWidget.h>
#include <softel/wt/ForexWidget.h>
#include <softel/wt/ItemTradeWidget.h>
#include <softel/wt/TallentTradeWidget.h>
#include <softel/wt/BitcoinTradeWidget.h>

#include <softel/wt/InfoWidget.h>
#include <softel/wt/UinfoWidget.h>


using namespace std;
using namespace SF::CORE;
using namespace SF::TRADE;
using namespace Wt;

namespace SF {
namespace WT {


MainPanel::MainPanel(const Wt::WEnvironment& env)
: WApplication(env) {

    setTitle(Machines::tm.desc);

    useStyleSheet("style/main.css");

    messageResourceBundle().use(appRoot() + "layout");

    WTabWidget* tab = new WTabWidget(root());

    BaseTradeWidget* widgetTrade;

    // Set tabs
    ILOGF << "Starting machine: " << SF::TRADE::toString(Machines::tm) << std::endl;

    if (Machines::tm.type == TRADING_WIDGET) {
        widgetTrade = new TradingWidget();
    } else if (Machines::tm.type == FOREX_WIDGET) {
        widgetTrade = new ForexWidget();
    } else if (Machines::tm.type == ITEM_TRADE_WIDGET) {
        widgetTrade = new ItemTradeWidget();
    } else if (Machines::tm.type == TALLENT_TRADE_WIDGET) {
        widgetTrade = new TallentTradeWidget();
    } else if (Machines::tm.type == BITCOIN_TRADE_WIDGET) {
        widgetTrade = new BitcoinTradeWidget();
    }
    tab->addTab(widgetTrade, Machines::tm.desc, Wt::WTabWidget::PreLoading);
    tab->addTab(new InfoWidget(widgetTrade->getTradeMachine()), "Info", Wt::WTabWidget::PreLoading);
    tab->addTab(new UinfoWidget(), "UserHistory", Wt::WTabWidget::PreLoading);
    
    tab->setStyleClass("tabwidget");
}

MainPanel::~MainPanel() {
    // TODO Auto-generated destructor stub
}

} /* namespace WT */
} /* namespace SF */
