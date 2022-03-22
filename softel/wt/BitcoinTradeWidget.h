/* 
 * File:   BitcoinTradeWidget.h
 * Author: stan
 *
 * Created on October 8, 2016, 6:26 PM
 * 
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 * 
 */

#ifndef SOFTEL_WT_BITCOINTRADEWIDGET_H_
#define SOFTEL_WT_BITCOINTRADEWIDGET_H_

#include <Wt/WContainerWidget>
#include <Wt/WTable>

#include <softel/core/common.h>

#include "BaseTradeWidget.h"
#include "TradeMachine.h"
#include "OrderMarketWidget.h"


namespace SF {
namespace WT {


class BitcoinTradeWidget : public BaseTradeWidget {
public:
    explicit BitcoinTradeWidget(Wt::WContainerWidget *parent = 0);
    virtual ~BitcoinTradeWidget();
private:
    void start();
};

class OrderBitcoinWidget : public OrderMarketWidget {
public:
    OrderBitcoinWidget(std::shared_ptr<TradeMachine> tradeMachine, Wt::WContainerWidget *parent = 0);
    virtual ~OrderBitcoinWidget() {}
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_BITCOINTRADEWIDGET_H_ */

