/*
 * OrderMarketWidget.cc
 *
 *  Created on: Mar 6, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include "OrderMarketWidget.h"

#include <softel/core/commonDefs.h>
#include <softel/core/common.h>
#include "common.h"

#include "TradeMachine.h"

using namespace std;
using namespace SF::CORE;
using namespace SF::TRADE;
using namespace Wt;

using SF::TRADE::THRIFT::Order;
using SF::TRADE::THRIFT::Symbol;

namespace SF {
namespace WT {


OrderMarketWidget::OrderMarketWidget(std::shared_ptr<TradeMachine> tradeMachine, WContainerWidget *parent)
: BaseOrderWidget(tradeMachine, parent)
{
    init();
}

OrderMarketWidget::~OrderMarketWidget()
{
    Wt::WApplication::instance()->enableUpdates(false);
}

void OrderMarketWidget::updateSymbol() {
    if (!symbols_.empty()) {
        tradeMachine_->getAvgMarketData(md_);
        WComboBox* cbSymbol = dynamic_cast<WComboBox*>(cbSymbol_);
        const Symbol& s = symbols_[cbSymbol->currentIndex()];
        symbolDesc_->update(s, md_["avg"]);
        txtSymbolMeasure_->setText(s.measure);
    } else {
        symbolDesc_->clear();
    }
}

void OrderMarketWidget::updateUserInfo()
{
    THRIFT::User u = users_[cbUser_->currentIndex()];
    std::stringstream ss;
    //ss.precision(2);
    ss << "<p>" << u.username << "</p>";
    for (auto & w : u.wallets) {
        ss << "<p>" << w.second.name << " " << std::fixed << Volume::get(w.second.volume) << " " << w.second.currency.name << "</p>";
    }
    txtUserInfo_->setText(ss.str());
}

void OrderMarketWidget::update() {
    //DLOGF << std::endl;

    WComboBox* cbSymbol = dynamic_cast<WComboBox*>(cbSymbol_);
    WComboBox* cbSymbolDst = dynamic_cast<WComboBox*>(cbSymbolDst_);

    tradeMachine_->getUsers(users_);
    updateComboBox(users_, cbUser_, true);

    tradeMachine_->getSymbolsSrc(symbols_);
    updateComboBox(symbols_, cbSymbol);

    tradeMachine_->getSymbolsDst(symbolsDst_);
    updateComboBox(symbolsDst_, cbSymbolDst);

    updateSymbol();
    updateUserInfo();
}

void OrderMarketWidget::init() {
    DLOGF << std::endl;

    cbSymbol_ = new WComboBox();
    WComboBox* cbSymbol = dynamic_cast<WComboBox*>(cbSymbol_);

    symbolDesc_ = new SymbolDescriptionWidget();

    cbSymbol->changed().connect(std::bind([=] () {
        Wt::log("info") << "cbSymbol->changed " << __FUNCTION__ << "::" << __LINE__;
        updateSymbol();
    }));

    cbSymbolDst_ = new WComboBox();
    WComboBox* cbSymbolDst = dynamic_cast<WComboBox*>(cbSymbolDst_);

    spVolume_ = new WDoubleSpinBox();
    spVolume_->setRange(0, 100);
    spVolume_->setValue(1);
    spVolume_->setDecimals(2);
    spVolume_->setSingleStep(0.1);

    spPrice_ = new WDoubleSpinBox();
    spPrice_->setValue(2.345);
    spPrice_->setDecimals(5);
    spPrice_->setSingleStep(0.1);

    btnSell_ = new WPushButton("Sell");
    btnBuy_ = new WPushButton("Buy");

    txtResult_ = new WText();
    txtSymbolMeasure_ = new WText();

    Wt::WTemplate *t = new Wt::WTemplate(Wt::WString::tr("OrderMarket"));
    addWidget(t);

    // TODO: move to separate trd mng component
    cbUser_ = new WComboBox();
    txtUserInfo_ = new WText();
    WPushButton *btnClear = new WPushButton("Clear");
    WPushButton *btnStartDemo = new WPushButton("Start Demo");
    t->bindWidget("user-edit", cbUser_);
    t->bindWidget("user-info", txtUserInfo_);
    t->bindWidget("clear-button", btnClear);
    t->bindWidget("demo-start-button", btnStartDemo);

    t->bindWidget("symbol-edit", cbSymbol_);
    t->bindWidget("symbol-desc", symbolDesc_);
    t->bindWidget("symbol-measure", txtSymbolMeasure_);
    t->bindWidget("symbolDst-edit", cbSymbolDst_);
    t->bindWidget("price-edit", spPrice_);
    t->bindWidget("volume-edit", spVolume_);
    t->bindWidget("sell-button", btnSell_);
    t->bindWidget("buy-button", btnBuy_);
    t->bindWidget("result-edit", txtResult_);

    cbUser_->changed().connect(std::bind([=] () {
        Wt::log("info") << "cbUser_->changed " << __FUNCTION__ << "::" << __LINE__;
        updateUserInfo();
    }));
    
    btnClear->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnClear " << __FUNCTION__ << "::" << __LINE__;
        tradeMachine_->clear();
    });

    btnStartDemo->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnStartDemo " << __FUNCTION__ << "::" << __LINE__;
        tradeMachine_->startDemo();
    });

    btnSell_->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnSell " << __FUNCTION__ << "::" << __LINE__;

        Order order;
        order.symbol = symbols_[cbSymbol->currentIndex()];
        order.symbolDst = symbolsDst_[cbSymbolDst->currentIndex()];
        order.user = users_[cbUser_->currentIndex()];
        Price::set(order.price, spPrice_->value());
        Volume::set(order.volume, spVolume_->value());
        order.type = THRIFT::OrderType::Sell;
        tradeMachine_->postOrder(order);

        txtResult_->setText(TRADE::toString(order));

        Wt::log("info") << "btnSell " << __FUNCTION__ << "::" << __LINE__;
    });

    btnBuy_->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnBuy " << __FUNCTION__ << "::" << __LINE__;
        txtResult_->setText("btnBuy");

        Order order;
        order.symbol = symbols_[cbSymbol->currentIndex()];
        order.symbolDst = symbolsDst_[cbSymbolDst->currentIndex()];
        order.user = users_[cbUser_->currentIndex()];
        Price::set(order.price, spPrice_->value());
        Volume::set(order.volume, spVolume_->value());
        order.type = THRIFT::OrderType::Buy;
        tradeMachine_->postOrder(order);

        txtResult_->setText(TRADE::toString(order));

        Wt::log("info") << "btnBuy " << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;
    });

}


} /* namespace WT */
} /* namespace SF */


