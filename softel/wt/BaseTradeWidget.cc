/*
 * BaseTradeWidget.cc
 *
 *  Created on: Mar 12, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BaseTradeWidget.h"

#include <softel/core/common.h>
#include <softel/trade/OrderBook.h>
#include "common.h"

using namespace std;
using namespace Wt;
using namespace SF::CORE::LOCK;
using namespace SF::TRADE;
using namespace SF::TRADE::THRIFT;


namespace SF {
namespace WT {


BaseTradeWidget::BaseTradeWidget(WContainerWidget *parent)
: WContainerWidget(parent)
{
    fnUpdater_ = [&] () {
        THRIFT::OrderBook book;
        tradeMachine_->getOrderBook(book);

        const TOrdersFull& orders = book.ordersOpen;
        //Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << " orders size = " << orders.size();

        const TTransactions& transactions = book.transactions["all"];
        //Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << " transactions size = " << transactions.size();

        tableTrade_->clear();
        tableTrade_->elementAt(0, 0)->addWidget(new Wt::WText("Buy"));
        tableTrade_->elementAt(0, 1)->addWidget(new Wt::WText("Sell"));
        tableTrade_->elementAt(0, 2)->addWidget(new Wt::WText("Transactions"));

        int b = 0;
        int s = 0;
        int t = 0;

        const TTransactions& tLM = book.transactions["last"];
        for (size_t i = 0; i < tLM.size(); i++) {
            const Transaction& transaction = tLM[i];
            //DLOGF << "tLM[" << i << "] " << transaction.toString() << std::endl;

            tableTrade_->elementAt(++b, 0)->addWidget(new Wt::WText(TRADE::toString(transaction.buy)));
            tableTrade_->elementAt(++s, 1)->addWidget(new Wt::WText(TRADE::toString(transaction.sell)));
            tableTrade_->elementAt(++t, 2)->addWidget(new Wt::WText(TRADE::toString(transaction)));

            tableTrade_->rowAt(t)->setStyleClass("ks-layouts-Grid-5");
        }

        for (auto itSymDst = orders.begin(); itSymDst != orders.end(); ++itSymDst) {
            for (auto itSym = itSymDst->second.begin(); itSym != itSymDst->second.end(); ++itSym) {

                auto itBuy = itSym->second.find(THRIFT::OrderType::Buy);
                if (itBuy != itSym->second.end()) {
                    for (auto it = itBuy->second.begin(); it != itBuy->second.end(); ++it) {
                        const Order& order = *it;
                        tableTrade_->elementAt(++b, 0)->addWidget(new Wt::WText(TRADE::toString(order)));
                        int row = (b > s) ? b : s;
                        tableTrade_->rowAt(row)->setStyleClass("ks-layouts-Grid-" + std::to_string(row % 2));
                    }
                }

                auto itSell = itSym->second.find(THRIFT::OrderType::Sell);
                if (itSell != itSym->second.end()) {
                    for (auto it = itSell->second.begin(); it != itSell->second.end(); ++it) {
                        const Order& order = *it;
                        tableTrade_->elementAt(++s, 1)->addWidget(new Wt::WText(TRADE::toString(order)));
                        int row = (b > s) ? b : s;
                        tableTrade_->rowAt(row)->setStyleClass("ks-layouts-Grid-" + std::to_string(row % 2));
                    }
                }
            }
        }

        for (auto it = transactions.begin(); it != transactions.end(); ++it) {
            const Transaction& transaction = *it;
            tableTrade_->elementAt(++t, 2)->addWidget(new Wt::WText(TRADE::toString(transaction)));
            tableTrade_->rowAt(t)->setStyleClass("ks-layouts-Grid-" + std::to_string(t % 2));
        }

        tableTradeHistory_->clear();
        tableTradeHistory_->elementAt(0, 0)->addWidget(new Wt::WText("Buy"));
        tableTradeHistory_->elementAt(0, 1)->addWidget(new Wt::WText("Sell"));

        const TOrders& ordersHistory = book.orders["history"];
        //Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << " ordersHistory size = " << ordersHistory.size();

        b = 0;
        s = 0;

        for (auto it = ordersHistory.begin(); it != ordersHistory.end(); ++it) {
            const Order& order = *it;
            switch (order.type) {
                case THRIFT::OrderType::Buy :
                    tableTradeHistory_->elementAt(++b, 0)->addWidget(new Wt::WText(TRADE::toString(order)));
                    break;

                case THRIFT::OrderType::Sell :
                    tableTradeHistory_->elementAt(++s, 1)->addWidget(new Wt::WText(TRADE::toString(order)));
                    break;

                default:
                    break;
            }

            int row = (b > s) ? b : s;
            tableTradeHistory_->rowAt(row)->setStyleClass("ks-layouts-Grid-" + std::to_string(row % 2));

        }

        orders_->update();

        Wt::WApplication::instance()->triggerUpdate();
    };
}

BaseTradeWidget::~BaseTradeWidget() {
    Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;
    Machines::m->unsubscribe(this);
    Wt::WApplication::instance()->enableUpdates(false);
}


}
}

