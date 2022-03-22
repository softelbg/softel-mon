/*
 * TradingApplication.cc
 *
 *  Created on: Feb 13, 2016
 *      Author: stan
 */

#include "TradingApplication.h"
#include <softel/core/commonDefs.h>
#include <softel/core/common.h>
#include "common.h"
#include "TradingWidget.h"
#include "MarketDataWidget.h"
#include "EscrowWidget.h"
#include "TradingWidget.h"
#include "ForexWidget.h"
#include "ItemTradeWidget.h"

namespace SF {
namespace WT {

using namespace std;
using namespace SF::CORE;
using namespace SF::TRADE;
using namespace Wt;


TradingApplication::TradingApplication(const Wt::WEnvironment& env)
: WApplication(env) {
	// TODO Auto-generated constructor stub
	setTitle("P2P Trading");

	useStyleSheet(appRoot() + "style/ws.css");

	messageResourceBundle().use(appRoot() + "layout");

	WTabWidget* tab = new WTabWidget(root());

	// Set tabs
	tab->addTab(new TradingWidget(), "Trade Machine", Wt::WTabWidget::PreLoading);
	tab->addTab(new ForexWidget(), "Forex Machine", Wt::WTabWidget::PreLoading);
	tab->addTab(new ItemTradeWidget(), "Item Trade Machine", Wt::WTabWidget::PreLoading);
	tab->addTab(new MarketDataWidget(), "Market Data", Wt::WTabWidget::PreLoading);
	tab->addTab(new EscrowWidget(), "Escrow", Wt::WTabWidget::PreLoading);

	tab->setStyleClass("tabwidget");
}

TradingApplication::~TradingApplication() {
	// TODO Auto-generated destructor stub
}


} /* namespace WT */
} /* namespace SF */
