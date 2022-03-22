/*
 * NewMachineWidget.cc
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include "NewMachineWidget.h"

#include <softel/wt/common.h>
#include "NewMachine.h"


using namespace std;
using namespace SF::CORE;
using namespace SF::TRADE;
using namespace SF::TRADE::THRIFT;
using namespace Wt;


namespace SF {
namespace WT {

NewMachineWidget::NewMachineWidget(WContainerWidget *parent)
: WContainerWidget(parent) {
    start();
}

NewMachineWidget::~NewMachineWidget() {
    AdminMachines::admnewmachine->unsubscribe(this);
    Wt::WApplication::instance()->enableUpdates(false);
}

void NewMachineWidget::start()
{
    Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;

    // Enable server push
    Wt::WApplication::instance()->enableUpdates(true);

    WLineEdit* machineDescription = new WLineEdit();
    machineDescription->setWidth("20em");

    Wt::WText* txtResult = new Wt::WText();

    WComboBox* cbUser = new WComboBox();
    TUsers users;
    AdminMachines::admnewmachine->getUsers(users);
    for (auto it = users.begin(); it != users.end(); ++it) {
        cbUser->addItem(it->username);
    }
    if (users.size() > 1) {
        std::hash<std::string> hash_string;
        cbUser->setCurrentIndex(hash_string(Wt::WApplication::instance()->sessionId()) % users.size());
        AdminMachines::admnewmachine->setOwner(users[cbUser->currentIndex()]);
    }

    WComboBox* cbMachineType = new WComboBox();
    TTypes machineTypes;
    AdminMachines::admnewmachine->getTypes(machineTypes);
    for (auto it = machineTypes.begin(); it != machineTypes.end(); ++it) {
        cbMachineType->addItem(*it);
    }

    WPushButton *btnGo = new WPushButton("Go");
    WPushButton *btnKillall= new WPushButton("Kill All");

    btnGo->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnGo " << __FUNCTION__ << "::" << __LINE__;

        std::string machineType = AdminMachines::admnewmachine->getType(cbMachineType->currentIndex());
        TMSpec m;
        m.type = machineType;
        m.owner = AdminMachines::admnewmachine->getUser(cbUser->currentIndex());
        m.desc = machineDescription->text().toUTF8();

        txtResult->setText("Provisioning " + machineType + " please wait...");
        AdminMachines::admnewmachine->go(m);
        txtResult->setText("Provisioned: " + toString(m));
    });

    btnKillall->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnKillall " << __FUNCTION__ << "::" << __LINE__;

        AdminMachines::admnewmachine->killall();
    });

    Wt::WCheckBox* userFilterCheckBox = new Wt::WCheckBox("Filter by User");

    cbUser->changed().connect(std::bind([=] () {
        Wt::log("info") << "cbUser->changed " << __FUNCTION__ << "::" << __LINE__;
        User owner;
        owner.username = cbUser->valueText().toUTF8();
        AdminMachines::admnewmachine->setOwner(owner);
    }));

    userFilterCheckBox->changed().connect(std::bind([=] () {
        Wt::log("info") << "userFilterCheckBox->changed " << __FUNCTION__ << "::" << __LINE__;
        AdminMachines::admnewmachine->update();
    }));

    tableMachines_ = new Wt::WTable();
    tableMachines_->setHeaderCount(1);
    tableMachines_->setWidth(Wt::WLength("800px"));
    tableMachines_->setStyleClass("tableview");

    Wt::WTemplate *tm = new Wt::WTemplate(Wt::WString::tr("AdmNewMachine"));
    addWidget(tm);
    tm->bindWidget("user-edit", cbUser);
    tm->bindWidget("machine-edit", cbMachineType);
    tm->bindWidget("machine-desc-edit", machineDescription);
    tm->bindWidget("Go-button", btnGo);
    tm->bindWidget("killall-button", btnKillall);
    tm->bindWidget("machines-table", tableMachines_);
    tm->bindWidget("result-edit", txtResult);
    tm->bindWidget("userfilter-checkbox", userFilterCheckBox);

    // Widget Updates
    std::function<void()> fnUpdate = [=] () {
        TMachines machines;
        if (userFilterCheckBox->checkState() == Wt::CheckState::Checked) {
            AdminMachines::admnewmachine->getMachines(cbUser->currentIndex(), machines);
        } else {
            AdminMachines::admnewmachine->getMachines(machines);
        }

        Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << " machines size = " << machines.size();

        tableMachines_->clear();
        tableMachines_->elementAt(0, 0)->addWidget(new Wt::WText("Machine"));
        tableMachines_->elementAt(0, 1)->addWidget(new Wt::WText("Link"));

        int row = 0;
        for (auto it = machines.begin(); it != machines.end(); ++it) {
            const TMSpec& m = it->second;
            tableMachines_->elementAt(++row, 0)->addWidget(new Wt::WText(toString(m)));
            tableMachines_->elementAt(row, 1)->addWidget(new Wt::WText("<a href=\"" + m.link + "\" target=\"_blank\">" + m.type + "</a>"));

            WPushButton *btnKill = new WPushButton("X");
            btnKill->clicked().connect([=] (const WMouseEvent& e) {
                Wt::log("info") << "btnKill " << __FUNCTION__ << "::" << __LINE__;
                AdminMachines::admnewmachine->kill(m);
            });
            tableMachines_->elementAt(row, 2)->addWidget(btnKill);

            tableMachines_->rowAt(row)->setStyleClass("ks-layouts-Grid-" + std::to_string(row % 2));
        }

        Wt::WApplication::instance()->triggerUpdate();
    };

    AdminMachines::admnewmachine->subscribe(Wt::WApplication::instance()->sessionId(), this, fnUpdate);

    fnUpdate();

    Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;
}


} /* namespace WT */
} /* namespace SF */
