/*
 * TradeAdminPanel.cc
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "TradeAdminPanel.h"

#include <softel/core/commonDefs.h>
#include <softel/core/common.h>
#include <softel/wt/common.h>

#include "NewMachineWidget.h"
#include "BillingWidget.h"


using namespace std;
using namespace SF::CORE;
using namespace Wt;

namespace SF {
namespace WT {

TradeAdminPanel::TradeAdminPanel(const Wt::WEnvironment& env)
: WApplication(env) {

    setTitle("Trade Platforms Admin Panel");

    useStyleSheet("style/main.css");

    messageResourceBundle().use(appRoot() + "admlayout");

    WTabWidget* tab = new WTabWidget(root());

    // Set tabs
    tab->addTab(new NewMachineWidget(), "TMachine Box", Wt::WTabWidget::PreLoading);
    tab->addTab(new BillingWidget(), "Billing", Wt::WTabWidget::PreLoading);

    tab->setStyleClass("tabwidget");
}

TradeAdminPanel::~TradeAdminPanel() {
    // TODO Auto-generated destructor stub
}


} /* namespace WT */
} /* namespace SF */
