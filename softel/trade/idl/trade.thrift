/*
 * 	Main Trade THRIFT API
 *
 *  Created on: Mar 28, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */

namespace cpp SF.TRADE.THRIFT

// TODO: Consider splitting in multiple thrift files ...

exception TradeException {
  	1: string why;
}

//  --------------   Constants  ------------------
// User Types
const string USER_TYPE_TALLENT = 	"TallentUserType"

// User Tags
const string USER_SKILLS = 			"skills"
const string USER_LINKS = 			"links"

// Multipliers
const i32 LotsMultiplier = 		1000000
const i32 PriceMultiplier = 		1000000
const i32 MoneyMultiplier = 		100

// Types
typedef i64 Timestamp; // Unix timestamp most probably
typedef i64 Price;
typedef i64 Volume;
typedef i64 ID;
typedef string Guid;


// General Structs
struct Location {
	1: double latitude;
	2: double longitude;
}

enum SocialNetType {
	FB,
	Twitter,
	GooglePlus
}

struct SocialNet {
	1: SocialNetType type;
}

// Trade related
struct Symbol {
    1: string name;
    2: string measure;
    3: set<string> tags;
    4: map<string, string> kv;
}

struct Wallet {
	 1: string name;
	11: Symbol currency;
	21: Volume volume;
}

enum PrivilegeLevel {
	Normal,
	Premium,
	Support,
	Admin
}	

struct UserAttributes {
	 1: Location loc;
	11: bool active;
	21: map<string, SocialNet> social;
	
}

struct User {
	 1: ID id;
	11: string username;
			
	21: map<string, string> kv;
	31: map<string, map<string, string>> kvs;
	
	41: map<Symbol, Wallet> wallets;
	
	51: PrivilegeLevel level;
	
	61: UserAttributes attributes;
}

enum OrderState {
    Pending = 1,
    Open = 2,
    Closed = 4,
    Deleted = 8,
    Canceled = 16,
    Rejected = 32,
    Expired = 64
}

enum OrderType {
    Buy = 1,
    Sell = 2,
    BuyLimit = 4,
    SellLimit = 8,
    BuyStop = 16,
    SellStop = 32
}

struct Order {
     1: ID id;

    11: Symbol symbol;
    12: Symbol symbolDst;

    21: Price price;
    22: Price takeProfit;
    23: Price stopLoss;
    24: Price swap;
    25: Price profit;

    31: Volume volume;

    41: map<OrderState, Timestamp> time;

    51: OrderState state;
    52: OrderType type;

    61: User user;
}

enum TransactionState {
    Open,
    Closed,
    Canceled
}

struct Transaction {
	 1: ID id;
    11: Order sell;
    21: Order buy;

    31: Price price;
    41: Volume volume;

	51: TransactionState state;
	
    61: map<TransactionState, Timestamp> time;
}

struct MarketData {
	11: Symbol symbol;
	21: map<string, double> price;
	31: double volume;
	41: Timestamp timestamp;
}

// Typedefs of Order Book components. TODO: improve here...
typedef map<Symbol, MarketData> TMarketDataPoint;
typedef map<string, TMarketDataPoint> TMarketDataPointMap;
typedef list<Order> TOrders;
typedef list<Symbol> TSymbols;
typedef list<User> TUsers;
typedef list<Transaction> TTransactions;


typedef map<Symbol, map<Symbol, map<OrderType, TOrders>>> TOrdersFull;
typedef map<i64, i32> IdIdx;

struct OrderBook {
	1:  i64 id;

	11: TOrdersFull ordersOpen;
	
	21: map<string, TOrders> orders;
	22: map<string, IdIdx> orderIdx;
	
    31: map<string, TTransactions> transactions;

    41: map<string, TMarketDataPoint> md;
}

// Trade Machine Specification aka Template.
struct TMSpec {
	11: User owner;
    
    21: string type;
    31: string desc;
    
    41: string host;
    42: i32 port;

    51: list<string> cmd;
    
    61: string link;
    
    71: Guid guid;

    81: Timestamp timeCreation;
}

struct TradeMachine {
	11: TMSpec spec;
	
	21: OrderBook book;
	
	31: list<User> users;
	
	// Symbols, TODO: consider mapped symbol lists...
	41: list<Symbol> symbolsSrc;
	42: list<Symbol> symbolsDst;
}

// TODO: consider splitting multiple services/interfaces...
// TODO: Add user-related token as first arg to all methods for authorization.
service BaseMachine {

	// Admin API
	
	
	// TM
	TradeMachine getTM();
		
	// Users
	oneway void setUsers(1:list<User> users);
        oneway void addUser(1:User user);
	list<User> getUsers();
	User getUser(1:i32 idx);
	
	// Symbols
	oneway void setSymbols(1:list<Symbol> src, 2:list<Symbol> dst);
	oneway void setSymbolsSrc(1:list<Symbol> src);
	oneway void setSymbolsDst(1:list<Symbol> dst);
	list<Symbol> getSymbolsSrc();
	list<Symbol> getSymbolsDst();
	Symbol getSymbolSrc(1:i32 idx);
	Symbol getSymbolDst(1:i32 idx);
			
				
	// Trade API
	oneway void clearBook();
	i64 postOrder(1:Order o);
	i64 postOrderWithMatch(1:Order o, 2:string match);
	i64 matchOrders(1:ID id1, 2:ID id2);
	
	OrderBook getOrderBook();
	map<string, TMarketDataPoint> getMarketData();
	
	// Tasks/Properties API
	list<string> waitForUpdates(); // TODO: some async update mechanism needed...
}

// TM Administration
service AdminMachine {
	oneway void run(1:TMSpec tm);
    oneway void kill(1:TMSpec tm);
    oneway void killall();
}

