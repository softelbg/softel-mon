/*
 * BillingWidget.cc
 *
 *  Created on: Mar 15, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include "BillingWidget.h"

#include <softel/wt/common.h>
#include "NewMachine.h"

using namespace SF::TRADE;

using SF::TRADE::THRIFT::User;

namespace SF {
namespace WT {

BillingWidget::BillingWidget(WContainerWidget *parent)
: WContainerWidget(parent) {
    start();
}

BillingWidget::~BillingWidget() {
    AdminMachines::admnewmachine->unsubscribe(this);
    Wt::WApplication::instance()->enableUpdates(false);
}

void BillingWidget::start()
{
    Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;

    // Enable server push
    Wt::WApplication::instance()->enableUpdates(true);

    Wt::WText* txtResult = new Wt::WText(this);

    Wt::WTable* tableTypes = new Wt::WTable(this);
    tableTypes->setHeaderCount(1);
    tableTypes->setWidth(Wt::WLength("800px"));
    tableTypes->setStyleClass("tableview");

    Wt::WTable* tableMachines = new Wt::WTable(this);
    tableMachines->setHeaderCount(1);
    tableMachines->setWidth(Wt::WLength("800px"));
    tableMachines->setStyleClass("tableview");

    // Widget Updates
    std::function<void()> fnUpdate = [=] () {
        User owner;
        AdminMachines::admnewmachine->getOwner(owner);
        txtResult->setText("User: " + owner.username);

        TTypes types;
        AdminMachines::admnewmachine->getTypes(types);
        TMachines machines;
        AdminMachines::admnewmachine->getMyMachines(machines);

        int row = 0;
        tableTypes->clear();
        tableTypes->elementAt(0, 0)->addWidget(new Wt::WText("TM types"));
        for (auto it = types.begin(); it != types.end(); ++it) {
            tableTypes->elementAt(++row, 1)->addWidget(new Wt::WText(*it));
        }

        row = 0;
        tableMachines->clear();
        tableMachines->elementAt(0, 0)->addWidget(new Wt::WText("My Machines"));
        for (auto it = machines.begin(); it != machines.end(); ++it) {
            tableMachines->elementAt(++row, 1)->addWidget(new Wt::WText(toString(it->second)));
        }
        // TODO: dummy price based just on the count of the TMs... there is no real billing here...
        double price = machines.size() * 15;
        tableMachines->elementAt(++row, 1)->addWidget(new Wt::WText("Total: " + std::to_string(price) + " USD per month"));

        Wt::WApplication::instance()->triggerUpdate();
    };

    AdminMachines::admnewmachine->subscribe(Wt::WApplication::instance()->sessionId(), this, fnUpdate);

    fnUpdate();

    Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;
}


} /* namespace WT */
} /* namespace SF */
