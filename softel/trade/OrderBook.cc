/*
 * OrderBook.cc
 *
 *  Created on: Mar 28, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include <softel/trade/OrderBook.h>

using namespace SF::TRADE::THRIFT;

namespace SF {
namespace TRADE {


OrderBook::OrderBook(THRIFT::TradeMachine& tm)
: tm_(tm), book_(tm.book)
{
    book_.id = 0;
}

OrderBook::~OrderBook() {
    // TODO Auto-generated destructor stub
}

std::string OrderBook::toString() {
    std::stringstream ss;
    ss << "Order Book" << std::endl;
    book_.printTo(ss);
    ss << std::endl << std::endl;
    return ss.str();
}

void OrderBook::orderOpen(const THRIFT::Order& order) {
    THRIFT::TOrders& orders = book_.orders["open"];
    THRIFT::IdIdx& idx = book_.orderIdx["open"];

    orders.push_back(order);
    std::sort(orders.begin(), orders.end());
    // TODO: Testing... improve here...
    for (size_t i = 0; i < orders.size(); ++i) {
        THRIFT::Order& o = orders[i];
        idx[o.id] = i;
    }

    DLOG << "--------------" << std::endl;
    for (auto o : orders) {
        DLOG << o.symbol.name << " " << o.symbolDst.name << " " << OrderTypeToStr(o.type) << " " << Price::get(o.price) << std::endl;
    }
    DLOG << "-----------------------------" << std::endl;
}

void OrderBook::postOrder(Order& order) {
    order.id = book_.id++;
    order.time[THRIFT::OrderState::Open] = time(0);
    order.state = THRIFT::OrderState::Open;

    THRIFT::TOrders& orders = book_.ordersOpen[order.symbolDst][order.symbol][order.type];
    orders.push_back(order);
    book_.orders["history"].push_back(order);

    //orderOpen(order);

    matchOrders(order);
}

void OrderBook::matchOrders(const Order& order) {

    book_.orders["last"].clear();
    book_.transactions["last"].clear();

    TOrders& ordersBuy = book_.ordersOpen[order.symbolDst][order.symbol][THRIFT::OrderType::Buy];
    TOrders& ordersSell = book_.ordersOpen[order.symbolDst][order.symbol][THRIFT::OrderType::Sell];

    std::sort(ordersBuy.begin(), ordersBuy.end());
    std::sort(ordersSell.begin(), ordersSell.end());

    auto itBuy = ordersBuy.begin();
    auto itSell = ordersSell.begin();

    // TODO: Improve here...
    while(itBuy !=  ordersBuy.end() && itSell != ordersSell.end()) {
        if (itBuy->price < itSell->price) {
            // No price match.
            break;
        } else {
            // Price matched
            addTransaction(*itBuy, *itSell);

            if (itBuy->volume > itSell->volume) {
                itBuy->volume -= itSell->volume;
                book_.orders["last"].push_back(*itSell);
                ordersSell.erase(itSell);
                itSell = ordersSell.begin();
                continue;
            } else if (itBuy->volume == itSell->volume) {
                book_.orders["last"].push_back(*itSell);
                book_.orders["last"].push_back(*itBuy);
                ordersBuy.erase(itBuy);
                itBuy = ordersBuy.begin();
                ordersSell.erase(itSell);
                itSell = ordersSell.begin();
                continue;
            } else {
                itSell->volume -= itBuy->volume;
                book_.orders["last"].push_back(*itBuy);
                ordersBuy.erase(itBuy);
                itBuy = ordersBuy.begin();
                continue;
            }

            ++itBuy;
            ++itSell;
        }
    }

    // TODO: Set Bid/Ask here ...
    THRIFT::MarketData& md = book_.md["avg"][order.symbol];
    if (!ordersBuy.empty()) {
        md.price["bid"] = Price::get(ordersBuy[0].price);
    }
    if (!ordersSell.empty()) {
        md.price["ask"] = Price::get(ordersSell[0].price);
    }

    DLOGF << order.symbol.name << " bid[" << md.price["bid"] << "] ask[" << md.price["ask"] << "]" << std::endl;
    DLOGF << "Spread[" << order.symbol.name << "] = " << md.price["ask"] - md.price["bid"] << " " << order.symbolDst.name << std::endl;
}

void OrderBook::clear() {
    book_.id = 0;
    book_.ordersOpen.clear();
    book_.orders.clear();
    book_.transactions["all"].clear();
    book_.transactions["last"].clear();
}

void OrderBook::addTransaction(const Order& buy, const Order& sell) {
    // Check users
    if (equal(buy.user, sell.user)) {
        return;
    }

    THRIFT::Price p = (buy.price < sell.price) ? buy.price : sell.price;
    
    // Check for enough buyer money, move this check up...
    // TODO: Make proper wallet policy for exchanges...
    if (!buy.user.wallets.empty()) {
        auto walletBuy = buy.user.wallets.find(buy.symbolDst);
        if (walletBuy == buy.user.wallets.end() || walletBuy->second.volume < p) {
            // TODO: Throw here
            //ELOGF << "Not enough money for the transaction: " << buy;
            //return;
        }
    }

    // Checks completed, finalize the transaction
    Transaction t = SF::TRADE::newTransaction(buy, sell, tm_);
    book_.transactions["all"].push_back(t);
    book_.transactions["last"].push_back(t);

    // TODO: calc wavg prices ... The Order Book should be implemented in its full scale...
    // Currently emulating simple order book bid/ask prices calculation weighted avg price
    // The Active orders in Order Book should take into account as the buy/sell pressure matters
    THRIFT::MarketData& md = book_.md["avg"][buy.symbol];

    THRIFT::Price price1;
    Price::set(price1, md.price["price"]);

    THRIFT::Volume volume;
    Volume::set(volume, md.volume);
    THRIFT::Volume volumex = volume;
    volumex += t.volume;

    THRIFT::Price pricex = volume * price1;
    pricex += t.volume * t.price;
    pricex /= volumex;

    md.volume = Volume::get(volumex);
    md.price["price"] = Price::get(pricex);
}


} /* namespace TRADE */
} /* namespace SF */

