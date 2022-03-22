/*
 * OrderForexWidget.cc
 *
 *  Created on: Mar 16, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include <softel/wt/OrderForexWidget.h>

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

OrderForexWidget::OrderForexWidget(std::shared_ptr<TradeMachine> tradeMachine, WContainerWidget *parent)
: BaseOrderWidget(tradeMachine, parent)
{
    init();
}

OrderForexWidget::~OrderForexWidget()
{
    Wt::WApplication::instance()->enableUpdates(false);
}

void OrderForexWidget::init() {

    WComboBox* cbUser = new WComboBox();
    tradeMachine_->getUsers(users_);
    updateComboBox(users_, cbUser, true);

    cbSymbol_ = new WComboBox();
    WComboBox* cbSymbol = dynamic_cast<WComboBox*>(cbSymbol_);
    tradeMachine_->getSymbolsSrc(symbols_);
    addItemName(symbols_, cbSymbol);

    double step = 0.0001;

    WDoubleSpinBox* spVolume = new WDoubleSpinBox();
    spVolume->setRange(0,1000);
    spVolume->setValue(1);
    spVolume->setDecimals(2);
    spVolume->setSingleStep(0.1);

    spPrice_ = new WDoubleSpinBox();
    spPrice_->setValue(0);
    spPrice_->setDecimals(5);
    spPrice_->setSingleStep(step);

    spStopLoss_ = new WDoubleSpinBox();
    spStopLoss_->setValue(0);
    spStopLoss_->setDecimals(5);
    spStopLoss_->setSingleStep(step);
    spTakeProfit_ = new WDoubleSpinBox();
    spTakeProfit_->setValue(0);
    spTakeProfit_->setDecimals(5);
    spTakeProfit_->setSingleStep(step);

    cbType_ = new WComboBox();
    cbType_->addItem("Market Execution");
    cbType_->addItem("Pending Order");

    WComboBox* cbTypePending = new WComboBox();
    cbTypePending->addItem("Buy Limit");
    cbTypePending->addItem("Sell Limit");
    cbTypePending->addItem("Buy Stop");
    cbTypePending->addItem("Sell Stop");

    WPushButton *btnSell = new WPushButton("Sell by Market");
    WPushButton *btnBuy = new WPushButton("Buy by Market");
    WPushButton *btnPlace = new WPushButton("Place Order");
    WPushButton *btnClear = new WPushButton("Clear");

    WLineEdit* leComment = new WLineEdit();

    txtResult_ = new WText();

    txtBid_ = new WText();
    txtAsk_ = new WText();

    WText* txtSymVol = new WText();

    Wt::WDateEdit *deExpiry = new Wt::WDateEdit();

    Wt::WTemplate *tmplOrderForex = new Wt::WTemplate(Wt::WString::tr("OrderForex"));
    Wt::WTemplate *tmplOrderForexMarketExecution = new Wt::WTemplate(Wt::WString::tr("OrderForexMarketExecution"));
    Wt::WTemplate *tmplOrderForexPending = new Wt::WTemplate(Wt::WString::tr("OrderForexPending"));

    addWidget(tmplOrderForex);

    tmplOrderForex->bindWidget("clear-button", btnClear);
    tmplOrderForex->bindWidget("user-edit", cbUser);
    tmplOrderForex->bindWidget("symbol-edit", cbSymbol_);
    tmplOrderForex->bindWidget("volume-edit", spVolume);
    tmplOrderForex->bindWidget("sl-edit", spStopLoss_);
    tmplOrderForex->bindWidget("tp-edit", spTakeProfit_);
    tmplOrderForex->bindWidget("comment-edit", leComment);
    tmplOrderForex->bindWidget("type-edit", cbType_);
    tmplOrderForex->bindWidget("result-edit", txtResult_);

    tmplOrderForexMarketExecution->bindWidget("bid-print", txtBid_);
    tmplOrderForexMarketExecution->bindWidget("ask-print", txtAsk_);
    tmplOrderForexMarketExecution->bindWidget("sell-button", btnSell);
    tmplOrderForexMarketExecution->bindWidget("buy-button", btnBuy);

    tmplOrderForexPending->bindWidget("typePending-edit", cbTypePending);
    tmplOrderForexPending->bindWidget("symbolvolume-print", txtSymVol);
    tmplOrderForexPending->bindWidget("price-edit", spPrice_);
    tmplOrderForexPending->bindWidget("place-button", btnPlace);
    tmplOrderForexPending->bindWidget("expiry-edit", deExpiry);
    tmplOrderForexPending->hide();
    //tmplOrderForexPending->bindWidget("expiry-edit", dateExpiry);

    tmplOrderForex->bindWidget("MarketExecution-type", tmplOrderForexMarketExecution);
    tmplOrderForex->bindWidget("Pending-type", tmplOrderForexPending);

    cbSymbol_->changed().connect(std::bind([=] () {
        Wt::log("info") << "cbSymbol_->changed " << __FUNCTION__ << "::" << __LINE__;
        txtSymVol->setText(cbSymbol->valueText().toUTF8() + " " + std::to_string(spVolume->value()));
    }));

    spVolume->changed().connect(std::bind([=] () {
        Wt::log("info") << "spVolume->changed " << __FUNCTION__ << "::" << __LINE__;
        txtSymVol->setText(cbSymbol->valueText().toUTF8() + " " + std::to_string(spVolume->value()));
    }));

    cbType_->changed().connect(std::bind([=] () {
        Wt::log("info") << "cbType->changed " << __FUNCTION__ << "::" << __LINE__;
        switch (cbType_->currentIndex()) {
            case 0:
                ILOGF << "changing to tmplOrderForexMarketExecution" << std::endl;
                tmplOrderForexMarketExecution->show();
                tmplOrderForexPending->hide();
                ILOGF << "changed to tmplOrderForexMarketExecution" << std::endl;
                break;
            case 1:
                ILOGF << "changing to tmplOrderForexPending" << std::endl;
                tmplOrderForexMarketExecution->hide();
                tmplOrderForexPending->show();
                txtSymVol->setText(cbSymbol->valueText().toUTF8() + " " + std::to_string(spVolume->value()));
                ILOGF << "changed to tmplOrderForexPending" << std::endl;
                break;
            default:
                tmplOrderForexMarketExecution->show();
                tmplOrderForexPending->hide();

                break;
        }
    }));

    btnClear->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnClear " << __FUNCTION__ << "::" << __LINE__;
        tradeMachine_->clear();
    });

    btnSell->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnSell " << __FUNCTION__ << "::" << __LINE__;

        Order order;
        order.symbol.name = cbSymbol->valueText().toUTF8();
        order.symbolDst = order.symbol;
        order.user.username = cbUser->valueText().toUTF8();
        Price::set(order.price, spPrice_->value());
        Volume::set(order.volume, spVolume->value());
        order.type = THRIFT::OrderType::Sell;
        tradeMachine_->postOrder(order);

        txtResult_->setText(TRADE::toString(order));

        Wt::log("info") << "btnSell " << __FUNCTION__ << "::" << __LINE__;
    });

    btnBuy->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnBuy " << __FUNCTION__ << "::" << __LINE__;
        txtResult_->setText("btnBuy");

        Order order;
        order.symbol.name = cbSymbol->valueText().toUTF8();
        order.symbolDst = order.symbol;
        order.user.username = cbUser->valueText().toUTF8();
        Price::set(order.price, spPrice_->value());
        Volume::set(order.volume, spVolume->value());
        order.type = THRIFT::OrderType::Buy;
        tradeMachine_->postOrder(order);

        txtResult_->setText(TRADE::toString(order));

        Wt::log("info") << "btnBuy " << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;
    });

}

void OrderForexWidget::updateMarketData(const TMarketDataPoint& md) {
    if (cbType_->currentIndex() == 0) {
        WComboBox* cbSymbol = dynamic_cast<WComboBox*>(cbSymbol_);
        Symbol symbol = newSymbol(cbSymbol->valueText().toUTF8());

        ILOGF << "symbol = " << symbol.name << std::endl;

        auto it = md.find(symbol);
        if (it != md.end()) {
            txtBid_->setText(std::to_string(it->second.bid()));
            txtAsk_->setText(std::to_string(it->second.ask()));

            // Initial set of the price values.
            if (spPrice_->value() == 0) {
                spPrice_->setValue((it->second.bid() + it->second.ask()) / 2);
            }
            if (spStopLoss_->value() == 0) {
                spStopLoss_->setValue((it->second.bid() + it->second.ask()) / 2);
            }
            if (spTakeProfit_->value() == 0) {
                spTakeProfit_->setValue((it->second.bid() + it->second.ask()) / 2);
            }
        }
    }
}


} /* namespace WT */
} /* namespace SF */
