/*
 * BaseOrderWidget.cc
 *
 *  Created on: Mar 15, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include "BaseOrderWidget.h"

#include <softel/core/commonDefs.h>
#include <softel/core/common.h>
#include "common.h"

#include "TradeMachine.h"

using namespace std;
using namespace SF::CORE;
using namespace SF::TRADE;
using namespace SF::TRADE::THRIFT;
using namespace Wt;

namespace SF {
namespace WT {

void addItemName(const SF::TRADE::THRIFT::TSymbols& container, Wt::WComboBox* cb) {
    for (auto it = container.begin(); it != container.end(); ++it) {
        cb->addItem(it->name);
    }
}
void addItemName(const SF::TRADE::THRIFT::TUsers& container, Wt::WComboBox* cb) {
    for (auto it = container.begin(); it != container.end(); ++it) {
        cb->addItem(it->username);
    }
}

BaseOrderWidget::BaseOrderWidget(std::shared_ptr<TradeMachine> tradeMachine, WContainerWidget *parent)
: WContainerWidget(parent), tradeMachine_(tradeMachine)
{
}

BaseOrderWidget::~BaseOrderWidget() {
    // TODO Auto-generated destructor stub
}


void BaseOrderWidget::setSymbol(const std::string& symbol) {
    cbSymbol_->setValueText(symbol);
}

void BaseOrderWidget::setSymbolDst(const std::string& symbol) {
    cbSymbolDst_->setValueText(symbol);
}

void BaseOrderWidget::setPrice(double price) {
    spPrice_->setValue(price);
}

void BaseOrderWidget::setPrice(const std::string& symbol, double price) {
    // Set Price for ComboBox-ed Symbols
    WComboBox* cbSymbol = dynamic_cast<WComboBox*>(cbSymbol_);
    if (cbSymbol) {
        int pos = cbSymbol->currentIndex();
        if (pos >= 0 && pos < symbols_.size() && symbols_[pos].name == symbol) {
            setPrice(price);
        }
    }
}

void BaseOrderWidget::update() {
    DLOGF << std::endl;
}


SymbolDescriptionWidget::SymbolDescriptionWidget(Wt::WContainerWidget* parent)
: WContainerWidget(parent) {

    txtSymbolDesc_ = new Wt::WText();
    imgSymbolDesc_ = new Wt::WImage();
    txtSymbolDescPrice_ = new Wt::WText();

    txtSymbolDescPrice_->setStyleClass("ks-font-3-bold");

    addWidget(txtSymbolDesc_);
    addWidget(imgSymbolDesc_);
    addWidget(txtSymbolDescPrice_);
}

SymbolDescriptionWidget::~SymbolDescriptionWidget() {
}

void SymbolDescriptionWidget::update(const Symbol& symbol) {
    THRIFT::TMarketDataPoint md;
    update(symbol, md);
}

void SymbolDescriptionWidget::update(const Symbol& symbol, THRIFT::TMarketDataPoint& md) {
    txtSymbolDescPrice_->setText(
            "BID " + std::to_string(md[symbol].price["bid"]) +
            " ASK " +  std::to_string(md[symbol].price["ask"]) +
            " Price " +  std::to_string(md[symbol].price["price"]) +
            " Vol " +  std::to_string(md[symbol].volume)
    );

    txtSymbolDesc_->setText(SF::CORE::Helper::get(symbol.kv, string("desc")));
    imgSymbolDesc_->setImageLink(Wt::WLink(SF::CORE::Helper::get(symbol.kv, string("img"))));
}

void SymbolDescriptionWidget::clear() {
    Symbol sym;
    sym.name = "###";
    THRIFT::TMarketDataPoint md;
    update(sym, md);
}

} /* namespace WT */
} /* namespace SF */
