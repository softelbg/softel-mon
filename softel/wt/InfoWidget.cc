/*
 * InfoWidget.cc
 *
 *  Created on: Mar 15, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include "InfoWidget.h"
#include "common.h"

using namespace std;
using namespace SF::CORE;
using namespace SF::TRADE;
using namespace SF::TRADE::THRIFT;
using namespace Wt;

namespace SF {
namespace WT {

InfoWidget::InfoWidget(std::shared_ptr<TradeMachine> tradeMachine, WContainerWidget *parent)
: WContainerWidget(parent), tradeMachine_(tradeMachine)
{
    start();
}

InfoWidget::~InfoWidget() {
    tradeMachine_->unsubscribe(this);
    Wt::WApplication::instance()->enableUpdates(false);
}

void InfoWidget::start()
{
    Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;

    // Enable server push
    Wt::WApplication::instance()->enableUpdates(true);

    Wt::WText* txtResult = new Wt::WText(this);

    SymbolsManagement* manageSymbols = new SymbolsManagement(this);
    SymbolsManagement* manageSymbolsDst = new SymbolsManagement(this);

    // Widget Updates
    std::function<void()> fnUpdate = [=] () {
        //txtResult->setText("User: " + owner.username);

        TSymbols symbols;
        TSymbols symbolsDst;
        tradeMachine_->getSymbolsSrc(symbols);
        tradeMachine_->getSymbolsDst(symbolsDst);

        manageSymbols->update(symbols, [=](const TSymbols& s) {
            tradeMachine_->setSymbolsSrc(s);
        });
        manageSymbolsDst->update(symbolsDst, [=](const TSymbols& s) {
            tradeMachine_->setSymbolsDst(s);
        });

        Wt::WApplication::instance()->triggerUpdate();
    };

    tradeMachine_->subscribe(Wt::WApplication::instance()->sessionId(), this, fnUpdate);

    fnUpdate();

    Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;
}

SymbolsManagement::SymbolsManagement(Wt::WContainerWidget* parent)
: Wt::WContainerWidget(parent)
{
    tableSymbols_ = new WTable();
    tableSymbols_->setHeaderCount(1);
    tableSymbols_->setStyleClass("tableview");

    tm_ = new Wt::WTemplate(Wt::WString::tr("SymbolManagement"));
    addWidget(tm_);
    tm_->bindWidget("symbols-table", tableSymbols_);
}

SymbolsManagement::~SymbolsManagement() {
}

void SymbolsManagement::update(const TSymbols& symbols, const std::function<void(const TSymbols& s)> fnUpdate) {
    //DLOGF << "symbols size = " << symbols.size() << std::endl;

    int row = 0;
    tableSymbols_->clear();
    tableSymbols_->elementAt(0, 0)->addWidget(new Wt::WText("Symbols"));
    tableSymbols_->elementAt(0, 1)->addWidget(new Wt::WText("Manage"));
    for (auto it = symbols.begin(); it != symbols.end(); ++it) {
        tableSymbols_->elementAt(++row, 0)->addWidget(new Wt::WText(it->name));

        WPushButton *btnDel = new WPushButton("X");
        btnDel->clicked().connect([=] (const WMouseEvent& e) {
            Wt::log("info") << "btnDel " << __FUNCTION__ << "::" << __LINE__ << " row = " << row;
            TSymbols s = symbols;
            auto itSym = s.begin();
            advance(itSym, row - 1);
            s.erase(itSym);

            fnUpdate(s);
        });
        tableSymbols_->elementAt(row, 1)->addWidget(btnDel);

        tableSymbols_->rowAt(row)->setStyleClass("ks-layouts-Grid-" + std::to_string(row % 2));
    }

    WLineEdit* leSymName = new WLineEdit();
    WLineEdit* leSymMeasure = new WLineEdit();
    WLineEdit* leSymDesc = new WLineEdit();
    WLineEdit* leSymImgLink = new WLineEdit();

    WPushButton *btnAdd = new WPushButton("+");
    btnAdd->clicked().connect([=] (const WMouseEvent& e) {
        Wt::log("info") << "btnAdd " << __FUNCTION__ << "::" << __LINE__;
        TSymbols symbols2 = symbols;
        Symbol s;
        s.name = leSymName->text().toUTF8();
        s.measure = leSymMeasure->text().toUTF8();
        s.kv["desc"] = leSymDesc->text().toUTF8();
        s.kv["img"] = leSymImgLink->text().toUTF8();

        symbols2.push_back(s);

        fnUpdate(symbols2);
    });

    tm_->bindWidget("add-button", btnAdd);
    tm_->bindWidget("name-edit", leSymName);
    tm_->bindWidget("measure-edit", leSymMeasure);
    tm_->bindWidget("desc-edit", leSymDesc);
    tm_->bindWidget("imglink-edit", leSymImgLink);
}

} /* namespace WT */
} /* namespace SF */

