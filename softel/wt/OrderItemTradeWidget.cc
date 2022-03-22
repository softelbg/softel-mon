/*
 * OrderItemTradeWidget.cc
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "OrderItemTradeWidget.h"
#include "common.h"

using namespace std;
using namespace SF::CORE;
using namespace SF::TRADE;
using namespace Wt;

using SF::TRADE::THRIFT::Order;
using SF::TRADE::THRIFT::Symbol;

namespace SF {
namespace WT {

OrderItemTradeWidget::OrderItemTradeWidget(std::shared_ptr<TradeMachine> tradeMachine, Wt::WContainerWidget *parent)
: BaseOrderWidget(tradeMachine, parent)
{
    init();
}

OrderItemTradeWidget::~OrderItemTradeWidget() {
    Wt::WApplication::instance()->enableUpdates(false);
}

void OrderItemTradeWidget::init() {

    WComboBox* cbUser = new WComboBox();
    tradeMachine_->getUsers(users_);
    updateComboBox(users_, cbUser, true);

    cbSymbol_ = new WLineEdit();
    WLineEdit* cbSymbol = dynamic_cast<WLineEdit*>(cbSymbol_);

    cbSymbolDst_ = new WComboBox();
    WComboBox* cbSymbolDst = dynamic_cast<WComboBox*>(cbSymbolDst_);
    tradeMachine_->getSymbolsDst(symbolsDst_);
    addItemName(symbolsDst_, cbSymbolDst);

    WDoubleSpinBox* spVolume = new WDoubleSpinBox();
    spVolume->setRange(0,1000);
    spVolume->setValue(1);
    spVolume->setDecimals(2);
    spVolume->setSingleStep(1);

    spPrice_ = new WDoubleSpinBox();
    spPrice_->setValue(2.345);
    spPrice_->setDecimals(3);
    spPrice_->setSingleStep(0.1);

    WPushButton *btnSell = new WPushButton("Sell");
    WPushButton *btnBuy = new WPushButton("Buy");
    WPushButton *btnClear = new WPushButton("Clear");

    Wt::WTemplate *t = new Wt::WTemplate(Wt::WString::tr("OrderItem"));
    addWidget(t);

    t->bindWidget("user-edit", cbUser);
    t->bindWidget("symbol-edit", cbSymbol_);
    t->bindWidget("symbolDst-edit", cbSymbolDst_);
    t->bindWidget("price-edit", spPrice_);
    t->bindWidget("volume-edit", spVolume);
    t->bindWidget("sell-button", btnSell);
    t->bindWidget("buy-button", btnBuy);
    t->bindWidget("clear-button", btnClear);

    btnClear->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnClear " << __FUNCTION__ << "::" << __LINE__;
        tradeMachine_->clear();
    });

    btnSell->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnSell " << __FUNCTION__ << "::" << __LINE__;

        Order order;
        order.symbol.name = cbSymbol->text().toUTF8();
        order.symbolDst = tradeMachine_->getSymbolDst(cbSymbolDst->currentIndex());
        order.user = tradeMachine_->getUser(cbUser->currentIndex());
        Price::set(order.price, spPrice_->value());
        Volume::set(order.volume, spVolume->value());
        order.type = THRIFT::OrderType::Sell;
        tradeMachine_->postOrder(order);

        Wt::log("info") << "btnSell " << __FUNCTION__ << "::" << __LINE__;
    });

    btnBuy->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnBuy " << __FUNCTION__ << "::" << __LINE__;

        Order order;
        order.symbol.name = cbSymbol->text().toUTF8();
        order.symbolDst = tradeMachine_->getSymbolDst(cbSymbolDst->currentIndex());
        order.user = tradeMachine_->getUser(cbUser->currentIndex());
        Price::set(order.price, spPrice_->value());
        Volume::set(order.volume, spVolume->value());
        order.type = THRIFT::OrderType::Buy;
        tradeMachine_->postOrder(order);

        Wt::log("info") << "btnBuy " << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;
    });

}


} /* namespace WT */
} /* namespace SF */
