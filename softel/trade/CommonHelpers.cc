/*
 * CommonHelpers.cc
 *
 *  Created on: Apr 29, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */

#include <softel/trade/CommonHelpers.h>
#include <softel/core/common.h>
#include <softel/core/commonDefs.h>
#include <softel/math/BigInt.h>

#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/concurrency/Thread.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>


using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using namespace SF::CORE;


namespace SF {
namespace TRADE {


// TODO: Make proper GUID computation, based on host, time, input etc...
std::string makeGuid(const std::string& input) {
    return input + "-" + std::to_string(time(NULL));
}

// Symbol
bool THRIFT::Symbol::operator< (const THRIFT::Symbol& s2) const {
    return TRADE::operator< (*this, s2);
}
bool operator< (const THRIFT::Symbol& s1, const THRIFT::Symbol& s2) {
    return s1.name < s2.name;
}
bool operator> (const THRIFT::Symbol& s1, const THRIFT::Symbol& s2) {
    return s1.name > s2.name;
}
bool operator== (const THRIFT::Symbol& s1, const THRIFT::Symbol& s2) {
    bool r = false;
    for (auto & t: s1.tags) {
        if (s2.tags.count(t) > 0) {
            r = true;
            break;
        }
    }
    return s1.name == s2.name || r;
}

std::string toString(const THRIFT::Symbol& s) {
    std::stringstream ss;
    s.printTo(ss);
    return ss.str();
}

std::string toHtml(const THRIFT::Symbol& s) {
    std::stringstream ss;

    auto itDesc = s.kv.find("desc");
    if (itDesc != s.kv.end()) {
        ss << "<p><label>" << itDesc->second << "</label></p>";
    }

    if (!s.tags.empty()) {
        ss << "<p><label>";
        for (auto & t: s.tags) {
            ss << t << " ";
        }
        ss << "</label></p>";
    }

    auto itImg = s.kv.find("img");
    if (itImg != s.kv.end()) {
        ss << "<img src=\"" << itImg->second << "\">";
    }

    return ss.str();
}

THRIFT::Symbol newSymbol(const std::string& n) {
    THRIFT::Symbol s;
    s.name = n;
    return s;
}
THRIFT::Symbol newSymbol(const std::string& n, const std::string& m) {
    THRIFT::Symbol s;
    s.name = n;
    s.measure = m;
    return s;
}
THRIFT::Symbol newSymbol(const std::string& n, const std::string& m, const std::string& d, const std::string& img) {
    THRIFT::Symbol s;
    s.name = n;
    s.measure = m;
    s.kv["desc"] = d;
    s.kv["img"] = img;
    return s;
}
THRIFT::Symbol newSymbol(const std::string& n, const std::string& m, const std::map<std::string, std::string>& mapKV) {
    THRIFT::Symbol s;
    s.name = n;
    s.measure = m;
    s.kv = mapKV;
    return s;
}


// Order
bool THRIFT::Order::operator< (const THRIFT::Order& o2) const {
    return TRADE::operator< (*this, o2);
}
bool operator< (const THRIFT::Order& o1, const THRIFT::Order& o2) {
    // TODO: order comparator Sell vs Buy
    bool r = o1.type == THRIFT::OrderType::Sell;

    if (o1.symbol.operator<(o2.symbol)) {
        return r;
    } else if (o1.symbol > o2.symbol) {
        return !r;
    } else if (o1.symbolDst.operator<(o2.symbolDst)) {
        return r;
    }  else if (o1.symbolDst > o2.symbolDst) {
        return !r;
    } else if (operator==(o1.symbolDst, o2.symbolDst) && o1.type < o2.type) {
        return r;
    } else if (o1.price < o2.price) {
        return r;
    } else if (o1.price == o2.price && o1.volume < o2.volume) {
        return r;
    } else if (o1.price == o2.price && o1.volume == o2.volume &&
            Helper::get(o1.time, THRIFT::OrderState::Open) < Helper::get(o2.time, THRIFT::OrderState::Open)) {
        return r;
    }

    return !r;
}

std::string toString(const THRIFT::Order& o) {
    std::stringstream ss;

    auto timeOpen = Helper::get(o.time, THRIFT::OrderState::Open);

    ss << "[" << o.id << "] [" << o.user.username << "] ";
    ss << OrderTypeToStr(o.type) << " ";
    ss << Volume::get(o.volume) << " ";
    if (!o.symbol.measure.empty()) ss << "[" << o.symbol.measure << "] ";
    ss << o.symbol.name << " at " << Price::get(o.price) << " " << o.symbolDst.name << " ";
    //std::time_t tt = std::chrono::system_clock::to_time_t(timeOpen);
    ss << ctime(&timeOpen);

    return ss.str();
}

std::string OrderTypeToStr(THRIFT::OrderType::type type) {
    std::string ret;
    switch (type) {
        case THRIFT::OrderType::Buy:
            ret = "Buy";
            break;
        case THRIFT::OrderType::Sell:
            ret = "Sell";
            break;
        case THRIFT::OrderType::BuyLimit:
            ret = "BuyLimit";
            break;
        case THRIFT::OrderType::SellLimit:
            ret = "SellLimit";
            break;
        case THRIFT::OrderType::BuyStop:
            ret = "BuyStop";
            break;
        case THRIFT::OrderType::SellStop:
            ret = "SellStop";
            break;
        default:
            ret = "UNKNOW";
            break;
    }
    return ret;
}


// Transaction
std::string toString(const THRIFT::Transaction& tIn) {
    THRIFT::Transaction t = tIn;
    std::stringstream ss;
    ss << t.sell.user.username << " => " << t.buy.user.username;
    ss << " " << t.sell.symbol.name << " at price " << Price::get(t.price);
    ss << " " << t.sell.symbolDst.name << " volume " << Volume::get(t.volume) << " " << t.sell.symbol.measure;
    ss << " " << ctime(&t.time[THRIFT::TransactionState::Open]);
    return ss.str();
}

THRIFT::Transaction newTransaction(const THRIFT::Order& b, const THRIFT::Order& s, THRIFT::TradeMachine& tm) {
    //timeExec = std::chrono::high_resolution_clock::now();
    THRIFT::Transaction t;
    t.time[THRIFT::TransactionState::Open] = time(0);
    t.buy = b;
    t.sell = s;
    t.price = (b.price < s.price) ? b.price : s.price;
    t.volume = (b.volume < s.volume) ? b.volume : s.volume;

    
    // TODO: Make proper wallet policy for exchanges...
    // Transfer money/compensation from buyer to seller.
    // TODO: improve...
    int idxBuy = -1;
    int idxSell = -1;
    for (size_t i = 0; i < tm.users.size(); i++) {
       if (equal(tm.users[i], b.user)) {
           idxBuy = i;
           break;
       }
    }
    for (size_t i = 0; i < tm.users.size(); i++) {
       if (equal(tm.users[i], s.user)) {
           idxSell = i;
           break;
       }
    }
    if (idxBuy >= 0 && idxSell >= 0) {
        auto walletBuySrc = tm.users[idxBuy].wallets.find(b.symbol);
        auto walletSellSrc = tm.users[idxSell].wallets.find(s.symbol);
        auto walletBuyDst = tm.users[idxBuy].wallets.find(b.symbolDst);
        auto walletSellDst = tm.users[idxSell].wallets.find(s.symbolDst);
        //assert(walletBuySrc != tm.users[idxBuy].wallets.end());
        //assert(walletSellSrc != tm.users[idxSell].wallets.end());
        //assert(walletBuyDst != tm.users[idxBuy].wallets.end());
        //assert(walletSellDst != tm.users[idxSell].wallets.end());
        // TODO: Make proper wallet policy for exchanges...
        if (walletBuySrc != tm.users[idxBuy].wallets.end() &&
            walletSellSrc != tm.users[idxSell].wallets.end() &&
            walletBuyDst != tm.users[idxBuy].wallets.end() &&
            walletSellDst != tm.users[idxSell].wallets.end()    
            ) {
        
            // TODO: Rework all price/volume multipliers/precision
            int64_t pv = t.price * t.volume / THRIFT::g_trade_constants.PriceMultiplier;
            walletBuyDst->second.volume  -= pv;
            walletSellDst->second.volume += pv;

            walletBuySrc->second.volume += t.volume;
            walletSellSrc->second.volume -= t.volume;
        }
    } else {
        // TODO: Some error here
    }
    
    return t;
}


// User
std::string toString(const THRIFT::User& u) {
    std::stringstream ss;
    u.printTo(ss);
    return ss.str();
}
std::string toString(const THRIFT::User& u, const std::string& k) {
    std::stringstream ss;
    auto itKey = u.kvs.find(k);
    if (itKey != u.kvs.end()) {
        for (auto & val : itKey->second) {
            ss << "{" << val.first << " " << val.second << "} ";
        }
    }
    return ss.str();
}

bool equal(const THRIFT::User& u1, const THRIFT::User& u2) {
    return u1.id == u1.id && u1.username == u2.username;
}

THRIFT::User user(const std::string& n) {
    THRIFT::User u;
    u.username = n;
    return u;
}
THRIFT::User user(const std::string& n, const TKV& m1, const TKVS& m2) {
    THRIFT::User u;
    u.username = n;
    u.kv = m1;
    u.kvs = m2;
    return u;
}


// TMSpec
std::string toString(const THRIFT::TMSpec& tm) {
    std::stringstream ss;
    ss << ctime(&tm.timeCreation) << " [" << tm.guid << "] " << tm.desc;
    return ss.str();
}
// TODO: Some thrift TCLIProtocol could be made instead...
std::string serialize(const THRIFT::TMSpec& tm) {
    std::stringstream ss;
    ss << " m.guid=" << tm.guid;
    ss << " m.host=" << tm.host;
    ss << " m.port=" << tm.port;
    ss << " m.type=" << tm.type;
    ss << " m.desc=" << boost::replace_all_copy(tm.desc, " ", "%20");
    ss << " m.owner.username=" << tm.owner.username;
    return ss.str();
}
void deserialize(const SF::CORE::UTIL::Options& opt, THRIFT::TMSpec& tm) {
    tm.guid = opt.get("m.guid");
    tm.host = opt.get("m.host");
    tm.port = opt.getInt("m.port");
    tm.type = opt.get("m.type");
    tm.desc = boost::replace_all_copy(opt.get("m.desc"), "%20", " ");
    tm.owner.username = opt.get("m.owner.username");
}
THRIFT::TMSpec newTMSpec(const THRIFT::User& u, const std::string& t, const std::string& d) {
    THRIFT::TMSpec tm;
    tm.type = t;
    tm.desc = d;
    tm.owner = u;
    return tm;
}


// Thrift
// Using non-blocking event based server, still it blocks on this call despite the name.
void thriftServeNonBlocking(boost::shared_ptr<THRIFT::BaseMachineIf> handler, int port) {
    ILOGF << "Serve on port = " << port << std::endl;
    boost::shared_ptr<TProcessor> processor(new THRIFT::BaseMachineProcessor(handler));
    //boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    boost::shared_ptr<TProtocolFactory> protocolFactory(new TJSONProtocolFactory());
    boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());

    boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(15);
    boost::shared_ptr<PosixThreadFactory> threadFactory = boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
    threadManager->threadFactory(threadFactory);
    threadManager->start();
    TNonblockingServer server(processor, protocolFactory, port, threadManager);
    server.serve();
}
std::shared_ptr<SF::TRADE::THRIFT::BaseMachineClient> thriftClientNonBlocking(int port) {
    ILOGF << "Client connect to port = " << port << std::endl;

    std::shared_ptr<SF::TRADE::THRIFT::BaseMachineClient> client;

    while (true) {
        try {
            boost::shared_ptr<TTransport> socket(new TSocket("localhost", port));
            boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
            //boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
            boost::shared_ptr<TProtocol> protocol(new TJSONProtocol(transport));
            client.reset(new SF::TRADE::THRIFT::BaseMachineClient(protocol));
            transport->open();
            break;
        } catch (TException& tx) {
            ELOGF << "EXCEPTION: " << tx.what() << std::endl;
        }
        sleep(1);
    }

    return client;
}

// Factories
int thriftPort(int port) {
    return port + 2000; // TODO: make it better...
}
int zmqPort(int port) {
    return port + 4000;
}


} /* namespace TRADE */
} /* namespace SF */

