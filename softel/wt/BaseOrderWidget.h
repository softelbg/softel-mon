/*
 * BaseOrderWidget.h
 *
 *  Created on: Mar 15, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_BASEORDERWIDGET_H_
#define SOFTEL_WT_BASEORDERWIDGET_H_

#include <Wt/WContainerWidget>

#include "common.h"
#include <softel/core/common.h>
#include "TradeMachine.h"

namespace SF {
namespace WT {

void addItemName(const SF::TRADE::THRIFT::TSymbols& container, Wt::WComboBox* cb);
void addItemName(const SF::TRADE::THRIFT::TUsers& container, Wt::WComboBox* cb);
template<class Container>
void updateComboBox(const Container& container, Wt::WComboBox* cb, bool rnd = false, int initPos = -1) {
    int pos = cb->currentIndex();
    cb->clear();
    addItemName(container, cb);
    if (rnd && pos < 0 && initPos == -1) {
        std::hash<std::string> hash_string;
        cb->setCurrentIndex(hash_string(Wt::WApplication::instance()->sessionId()) % container.size());
    } else if (pos > 0) {
        cb->setCurrentIndex(pos);
    } else if (initPos > -1) {
        cb->setCurrentIndex(initPos);
    }
}

class SymbolDescriptionWidget : public Wt::WContainerWidget {
public:
    explicit SymbolDescriptionWidget(Wt::WContainerWidget *parent = 0);
    virtual ~SymbolDescriptionWidget();

    void update(const SF::TRADE::THRIFT::Symbol& symbol);
    void update(const SF::TRADE::THRIFT::Symbol& symbol, SF::TRADE::THRIFT::TMarketDataPoint& md);

    void clear();

private:
    Wt::WText* txtSymbolDesc_;
    Wt::WImage* imgSymbolDesc_;
    Wt::WText* txtSymbolDescPrice_;
};

class BaseOrderWidget : public Wt::WContainerWidget {
public:
    BaseOrderWidget(std::shared_ptr<TradeMachine> tradeMachine, Wt::WContainerWidget *parent = 0);
    virtual ~BaseOrderWidget();

    void setSymbol(const std::string& symbol);
    void setSymbolDst(const std::string& symbol);
    void setPrice(double price);
    void setPrice(const std::string& symbol, double price);

    virtual void update();

protected:
    Wt::WComboBox* cbUser_;
    Wt::WText* txtUserInfo_;
    Wt::WFormWidget* cbSymbol_;
    Wt::WText* txtSymbolMeasure_;
    Wt::WFormWidget* cbSymbolDst_;
    Wt::WDoubleSpinBox* spPrice_;
    Wt::WDoubleSpinBox* spVolume_;
    Wt::WText* txtResult_;
    SymbolDescriptionWidget* symbolDesc_;
    Wt::WPushButton* btnSell_;
    Wt::WPushButton* btnBuy_;

    SF::TRADE::THRIFT::TUsers users_;
    SF::TRADE::THRIFT::TSymbols symbols_;
    SF::TRADE::THRIFT::TSymbols symbolsDst_;
    SF::TRADE::THRIFT::TMarketDataPointMap md_;

    std::shared_ptr<TradeMachine> tradeMachine_;
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_BASEORDERWIDGET_H_ */
