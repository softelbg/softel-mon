/*
 * OrderTallentWidget.cc
 *
 *  Created on: Apr 13, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include <softel/wt/OrderTallentWidget.h>
#include <softel/wt/TallentMachine.h>
#include <softel/core/commonDefs.h>
#include <softel/core/common.h>
#include "common.h"

using namespace std;
using namespace SF::CORE;
using namespace SF::TRADE;
using namespace Wt;

using SF::TRADE::THRIFT::Order;
using SF::TRADE::THRIFT::User;
using SF::TRADE::THRIFT::Symbol;

namespace SF {
namespace WT {

OrderTallentWidget::OrderTallentWidget(std::shared_ptr<TradeMachine> tradeMachine, Wt::WContainerWidget *parent)
: BaseOrderWidget(tradeMachine, parent)
{
    init();
}

OrderTallentWidget::~OrderTallentWidget() {
    Wt::WApplication::instance()->enableUpdates(false);
}

void OrderTallentWidget::init() {

    cbUser_ = new WComboBox();
    tradeMachine_->getUsers(users_);
    for (auto it = users_.begin(); it != users_.end(); ++it) {
        cbUser_->addItem(it->username);
    }
    std::hash<std::string> hash_string;
    cbUser_->setCurrentIndex(hash_string(Wt::WApplication::instance()->sessionId()) % users_.size());

    cbSymbol_ = new WComboBox();
    WComboBox* cbSymbol = dynamic_cast<WComboBox*>(cbSymbol_);
    tradeMachine_->getSymbolsSrc(symbols_);
    for (auto it = symbols_.begin(); it != symbols_.end(); ++it) {
        cbSymbol->addItem(it->name);
    }

    cbSymbolDst_ = new WComboBox();
    WComboBox* cbSymbolDst = dynamic_cast<WComboBox*>(cbSymbolDst_);
    tradeMachine_->getSymbolsDst(symbolsDst_);
    for (auto it = symbolsDst_.begin(); it != symbolsDst_.end(); ++it) {
        cbSymbolDst->addItem(it->name);
    }

    WDoubleSpinBox* spVolume = new WDoubleSpinBox();
    spVolume->setRange(0, 1000);
    spVolume->setValue(40);
    spVolume->setDecimals(0);
    spVolume->setSingleStep(10);

    spPrice_ = new WDoubleSpinBox();
    spPrice_->setRange(0, 10000);
    spPrice_->setValue(100);
    spPrice_->setDecimals(0);
    spPrice_->setSingleStep(10);

    WPushButton *btnSell = new WPushButton("Sell");
    WPushButton *btnBuy = new WPushButton("Buy");
    WPushButton *btnClear = new WPushButton("Clear");
    WPushButton *btnStartDemo = new WPushButton("Start Demo");

    Wt::WTemplate *tmplOrder = new Wt::WTemplate(Wt::WString::tr("OrderTallent"));
    Wt::WTemplate *tmplViewTallent = new Wt::WTemplate(Wt::WString::tr("TallentView"));
    Wt::WTemplate *tmplViewHR = new Wt::WTemplate(Wt::WString::tr("HRView"));

    addWidget(tmplOrder);

    WText* skills = new WText();

    tmplViewTallent->bindWidget("skills-print", skills);
    tmplViewTallent->bindWidget("sell-button", btnSell);

    tmplViewHR->bindWidget("buy-button", btnBuy);

    tmplOrder->bindWidget("symbol-edit", cbSymbol_);
    tmplOrder->bindWidget("user-edit", cbUser_);
    tmplOrder->bindWidget("clear-button", btnClear);
    tmplOrder->bindWidget("demo-start-button", btnStartDemo);

    tmplOrder->bindWidget("price-edit", spPrice_);
    tmplOrder->bindWidget("symbolDst-edit", cbSymbolDst_);
    tmplOrder->bindWidget("volume-edit", spVolume);
    tmplOrder->bindWidget("symbol-measure", new WText("hours per week"));

    tmplOrder->bindWidget("tallent-view", tmplViewTallent);
    tmplOrder->bindWidget("hr-view", tmplViewHR);

    auto fnUpdateUser = [=]() {
        User u = tradeMachine_->getUser(cbUser_->currentIndex());
        if (u.kv.count("TallentUserType")) {
            tmplViewTallent->show();
            tmplViewHR->hide();

            skills->setText(TRADE::toString(u, "skills"));
        } else {
            tmplViewTallent->hide();
            tmplViewHR->show();
        }
    };

    fnUpdateUser();

    cbUser_->changed().connect(std::bind([=] () {
        Wt::log("info") << "cbUser_->changed " << __FUNCTION__ << "::" << __LINE__;
        fnUpdateUser();
    }));

    btnStartDemo->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnStartDemo " << __FUNCTION__ << "::" << __LINE__;
        tradeMachine_->startDemo();
    });

    btnClear->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnClear " << __FUNCTION__ << "::" << __LINE__;
        tradeMachine_->clear();
    });

    btnSell->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnSell " << __FUNCTION__ << "::" << __LINE__;

        User u = tradeMachine_->getUser(cbUser_->currentIndex());

        Order order;
        /*
        order.symbol.name = u.desc->toString();
        auto desc = dynamic_cast<TallentUserDescription*>(u.desc.get());
        for (auto & skill : desc->skills) {
            order.symbol.desc.tags.insert(skill.first);
        }
         */
        order.symbol = tradeMachine_->getSymbolSrc(cbSymbol->currentIndex());
        order.symbolDst = tradeMachine_->getSymbolDst(cbSymbolDst->currentIndex());
        order.user = u;
        Price::set(order.price, spPrice_->value());
        Volume::set(order.volume, spVolume->value());
        order.type = THRIFT::OrderType::Sell;
        tradeMachine_->postOrder(order);
    });

    btnBuy->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnBuy " << __FUNCTION__ << "::" << __LINE__;

        User u = tradeMachine_->getUser(cbUser_->currentIndex());

        Order order;
        order.symbol = tradeMachine_->getSymbolSrc(cbSymbol->currentIndex());
        order.symbolDst = tradeMachine_->getSymbolDst(cbSymbolDst->currentIndex());
        order.user = u;
        Price::set(order.price, spPrice_->value());
        Volume::set(order.volume, spVolume->value());
        order.type = THRIFT::OrderType::Buy;
        tradeMachine_->postOrder(order);
    });

}

} /* namespace WT */
} /* namespace SF */
