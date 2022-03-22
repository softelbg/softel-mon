/*
 * InfoWidget.h
 *
 *  Created on: Mar 15, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_INFOWIDGET_H_
#define SOFTEL_WT_INFOWIDGET_H_

#include <softel/wt/common.h>
#include <softel/wt/TradeMachine.h>

namespace SF {
namespace WT {

class SymbolsManagement : public Wt::WContainerWidget {
public:
    explicit SymbolsManagement(Wt::WContainerWidget *parent = 0);
    virtual ~SymbolsManagement();

    void update(const SF::TRADE::THRIFT::TSymbols& symbols, const std::function<void(const SF::TRADE::THRIFT::TSymbols& s)> fnUpdate);

private:
    Wt::WTable* tableSymbols_;
    Wt::WTemplate* tm_;
};

class InfoWidget : public Wt::WContainerWidget, public UpdateSubscriber {
public:
    explicit InfoWidget(std::shared_ptr<TradeMachine> tradeMachine, Wt::WContainerWidget *parent = 0);
    virtual ~InfoWidget();

private:
    void start();

    std::shared_ptr<TradeMachine> tradeMachine_;
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_INFOWIDGET_H_ */
