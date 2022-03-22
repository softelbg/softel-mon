/*
 * CommonHelpers.h
 *
 *  Created on: Apr 29, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_TRADE_COMMONHELPERS_H_
#define SOFTEL_TRADE_COMMONHELPERS_H_

#include <softel/core/common.h>
#include <softel/core/util/Options.h>
#include <softel/trade/idl/messages.pb.h>
#include <softel/trade/idl/gen-cpp/BaseMachine.h>
#include <softel/trade/idl/gen-cpp/trade_constants.h>

namespace SF {
namespace TRADE {

//typedef std::chrono::high_resolution_clock::time_point TimePoint;

typedef std::map<std::string, std::string> TKV;
typedef std::map<std::string, std::map<std::string, std::string>> TKVS;

std::string makeGuid(const std::string& input);

// Transaction
std::string toString(const THRIFT::Transaction& t);
THRIFT::Transaction newTransaction(const THRIFT::Order& b, const THRIFT::Order& s, THRIFT::TradeMachine& tm);


// Order
std::string OrderTypeToStr(THRIFT::OrderType::type type);
std::string toString(const THRIFT::Order& o);
bool operator< (const THRIFT::Order& o1, const THRIFT::Order& o2);


// Symbol
bool operator< (const THRIFT::Symbol& s1, const THRIFT::Symbol& s2);
bool operator== (const THRIFT::Symbol& s1, const THRIFT::Symbol& s2);

std::string toString(const THRIFT::Symbol& s);
std::string toHtml(const THRIFT::Symbol& s);

THRIFT::Symbol newSymbol(const std::string& n);
THRIFT::Symbol newSymbol(const std::string& n, const std::string& m);
THRIFT::Symbol newSymbol(const std::string& n, const std::string& m, const std::string& d, const std::string& img);
THRIFT::Symbol newSymbol(const std::string& n, const std::string& m, const std::map<std::string, std::string>& mapKV);


// User
std::string toString(const THRIFT::User& u);
std::string toString(const THRIFT::User& u, const std::string& k);

THRIFT::User user(const std::string& n);
THRIFT::User user(const std::string& n, const TKV& m1, const TKVS& m2);

bool equal (const THRIFT::User& u1, const THRIFT::User& u2);


// TMSpec
std::string toString(const THRIFT::TMSpec& tm);
std::string serialize(const THRIFT::TMSpec& tm);
void deserialize(const SF::CORE::UTIL::Options& opt, THRIFT::TMSpec& tm);
THRIFT::TMSpec newTMSpec(const THRIFT::User& u, const std::string& t, const std::string& d);



// Thrift
// Using non-blocking event based server, still it blocks on this call despite the name.
void thriftServeNonBlocking(boost::shared_ptr<THRIFT::BaseMachineIf> handler, int port);
std::shared_ptr<SF::TRADE::THRIFT::BaseMachineClient> thriftClientNonBlocking(int port);


// Factories
int thriftPort(int port);
int zmqPort(int port);



// TODO: Remove these...
class Price {
public:
    static void set(THRIFT::Price& m, double p) {
        m = static_cast<THRIFT::Price>(p * THRIFT::g_trade_constants.PriceMultiplier);
    }
    static double get(const THRIFT::Price& m) {
        return static_cast<double>(m) / THRIFT::g_trade_constants.PriceMultiplier;
    }
};

class Volume {
public:
    static void set(THRIFT::Volume& m, double p) {
        m = static_cast<THRIFT::Volume>(p * THRIFT::g_trade_constants.LotsMultiplier);
    }
    static double get(const THRIFT::Volume& m) {
        return static_cast<double>(m) / THRIFT::g_trade_constants.LotsMultiplier;
    }
};


typedef std::map<THRIFT::Symbol, MarketDataProto> TMarketDataPoint;


} /* namespace TRADE */
} /* namespace SF */

#endif /* SOFTEL_TRADE_COMMONHELPERS_H_ */
