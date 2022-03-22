/*
 * ItemTradeWidget.cc
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include "ItemTradeWidget.h"

#include <softel/core/common.h>
#include "common.h"

using namespace SF::TRADE;
using namespace SF::TRADE::THRIFT;

namespace SF {
namespace WT {

ItemTradeWidget::ItemTradeWidget(Wt::WContainerWidget *parent)
: BaseTradeWidget(parent)
{
    start();
}

ItemTradeWidget::~ItemTradeWidget() {
    Wt::log("info") << __FUNCTION__ << "::" << __LINE__;
}

void ItemTradeWidget::start()
{
    Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;

    Wt::WApplication *app = Wt::WApplication::instance();

    // Enable server push
    app->enableUpdates(true);

    orders_ = new OrderItemTradeWidget(Machines::m);

    tableTrade_ = new Wt::WTable();
    tableTrade_->setHeaderCount(1);
    tableTrade_->setWidth(Wt::WLength("100%"));

    tableTradeHistory_ = new Wt::WTable();
    tableTradeHistory_->setHeaderCount(1);
    tableTradeHistory_->setWidth(Wt::WLength("100%"));

    Wt::WTemplate *tm = new Wt::WTemplate(Wt::WString::tr("ItemTradeMachine"));
    addWidget(tm);
    tm->bindWidget("order-edit", orders_);
    tm->bindWidget("trade-table", tableTrade_);
    tm->bindWidget("tradeHistory-table", tableTradeHistory_);

    tradeMachine_ = Machines::m;

    fnUpdater_ = [&] () {
        THRIFT::OrderBook book;
        tradeMachine_->getOrderBook(book);

        const TOrdersFull& orders = book.ordersOpen;
        Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << " orders size = " << orders.size();

        const TTransactions& transactions = book.transactions["all"];
        Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << " transactions size = " << transactions.size();

        tableTrade_->clear();
        tableTrade_->elementAt(0, 0)->addWidget(new Wt::WText("Buy"));
        tableTrade_->elementAt(0, 1)->addWidget(new Wt::WText("Sell"));
        tableTrade_->elementAt(0, 2)->addWidget(new Wt::WText("Transactions"));

        int b = 0;
        int s = 0;
        int t = 0;

        OrderItemTradeWidget* itemOrdersControl = dynamic_cast<OrderItemTradeWidget*>(orders_);

        // Fill Order Table Column from container.
        std::function<void(TOrders::const_iterator begin, TOrders::const_iterator end, int col, int& row)> fnFillOrderTable =
                [=](TOrders::const_iterator begin, TOrders::const_iterator end, int col, int& row) {
            for (auto it = begin; it != end; ++it) {
                const Order& order = *it;

                Wt::WPushButton* btnMatch = new Wt::WPushButton("Match");
                btnMatch->clicked().connect([=] (const Wt::WMouseEvent& e) {
                    Wt::log("info") << "btnMatch " << __FUNCTION__ << "::" << __LINE__;
                    itemOrdersControl->setPrice(TRADE::Price::get(order.price));
                    itemOrdersControl->setSymbol(order.symbol.name);
                    itemOrdersControl->setSymbolDst(order.symbolDst.name);
                });

                tableTrade_->elementAt(++row, col)->addWidget(btnMatch);
                tableTrade_->elementAt(row, col)->addWidget(new Wt::WText(TRADE::toString(order)));
                tableTrade_->rowAt(row)->setStyleClass("ks-layouts-Grid-" + std::to_string(row % 2));
            }
        };

        for (auto itSymDst = orders.begin(); itSymDst != orders.end(); ++itSymDst) {
            for (auto itSym = itSymDst->second.begin(); itSym != itSymDst->second.end(); ++itSym) {
                auto itBuy = itSym->second.find(THRIFT::OrderType::Buy);
                if (itBuy != itSym->second.end()) {
                    fnFillOrderTable(itBuy->second.begin(), itBuy->second.end(), 0, b);
                }

                auto itSell = itSym->second.find(THRIFT::OrderType::Sell);
                if (itSell != itSym->second.end()) {
                    fnFillOrderTable(itSell->second.begin(), itSell->second.end(), 1, s);
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
        Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << " ordersHistory size = " << ordersHistory.size();

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

        Wt::WApplication::instance()->triggerUpdate();
    };

    Machines::m->subscribe(Wt::WApplication::instance()->sessionId(), this, fnUpdater_);
    fnUpdater_();
}


} /* namespace WT */
} /* namespace SF */
