/*
 * ModuleTrade.cc
 *
 *  Created on: Apr 22, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */

#include <softel/core/common.h>
#include <softel/core/commonDefs.h>
#include <softel/core/util/Options.h>
#include <softel/core/net/zmq/ZMQDuplex.hpp>
#include <softel/core/net/zmq/ZMQSubscription.h>
#include <softel/core/net/zmq/CGuardZMQMsg.hpp>
#include <softel/trade/idl/Protobuf.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/concurrency/Thread.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>

#include <softel/exec/zmqsub/ModuleTrade.h>
#include <softel/trade/TradeMachine.h>
#include <softel/trade/TBSONProtocol.h>
#include <softel/trade/TJSONProtocol.h>

using namespace std;
using namespace SF::CORE;
using namespace SF::CORE::UTIL;
using namespace SF::TRADE;
using namespace SF::TRADE::THRIFT;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

namespace SF {

ModuleTrade::ModuleTrade(const SF::CORE::UTIL::Options& opt)
: ModuleBase(opt),
  port_(new SF::CORE::UTIL::OptionParam<int>("port", 21981)),
  mode_(new SF::CORE::UTIL::OptionParam<std::string>("mode", "server")),
  proto_(new SF::CORE::UTIL::OptionParam<std::string>("proto", "thrift")),
  sub_(new SF::CORE::UTIL::OptionParam<std::string>("sub", "test")),
  count_(new SF::CORE::UTIL::OptionParam<int>("count", 100))
{
    setName("ModuleTrade");
    setInfo("Module Trade Testing.");

    params_.push_back(port_);
    params_.push_back(mode_);
    params_.push_back(proto_);
    params_.push_back(sub_);
    params_.push_back(count_);

    init();
    initFnMap();
}

ModuleTrade::~ModuleTrade() {
    // TODO Auto-generated destructor stub
}

void ModuleTrade::run()
{
    string keyFn = mode_->getValue() + "::" + proto_->getValue();
    DLOGF << "Call " << keyFn << endl;

    auto fn = mapFn_[keyFn];
    fn();
}

void ModuleTrade::initFnMap() {

    // THRIFT
    mapFn_["server::thrift"] = [&]() {
        DLOGF << endl;
        Options::TStr2Str m = { {"m.port", std::to_string(port_->getValue())} };
        Options opt(m.begin(), m.end());
        boost::shared_ptr<BaseMachineIf> handler(new SF::TRADE::TradeMachine(opt));

        int port = port_->getValue();
        DLOGF << "Port = " << port << std::endl;

        thriftServeNonBlocking(handler, port);
    };

    mapFn_["client::thrift"] = [&]() {
        DLOGF << "Port = " << port_->getValue() << std::endl;

        std::shared_ptr<BaseMachineClient> client = thriftClientNonBlocking(thriftPort(port_->getValue()));
        std::shared_ptr<SF::CORE::ZMQSubscription> zmqSub(
                new SF::CORE::ZMQSubscription("tcp://localhost:" + std::to_string(zmqPort(port_->getValue()))));

        std::atomic<bool> isWorkerStopped(false);

        std::thread workerZmqSub([&]() {
            while (!isWorkerStopped) {
                //ILOG << "Waiting ZMQ Update..." << std::endl;
                zmqSub->Receive(1000, [&](SF::CORE::CGuardZMQMsg& msg, const int size) {
                    if (size == 0) {
                        return;
                    }

                    //ILOG << "size = " << size << std::endl << SF::CORE::Helper::toHex((unsigned char*)msg.Data(), size);

                    UpdateMessageProto msgUpdate;
                    msgUpdate.ParseFromArray(msg.Data(), msg.DataSize());

                    ILOG << "ZMQ Update: " << msgUpdate.component() << std::endl;

                    monitor_.notify_all();
                });
            }
        });

        // TODO: Still thrift callback emulation (commet-like) fails...
        /*
        std::thread t2([&]() {
            while (true) {
                ILOG << "Waiting THRIFT Update..." << std::endl;

                std::vector<std::string> result;
                client->waitForUpdates(result);

                if (!result.empty() && result[0] != "timeout") {
                    ILOG << "THRIFT Update: ";
                    for (auto r : result) {
                        ILOG << " " << r;
                    }
                    ILOG << std::endl;

                    monitor_.notify_all();
                } else {
                    ILOG << "timeout" << std::endl;
                }
            }
        });
         */

        std::map<std::string, std::function<void()>> mapClientFn;

        mapClientFn["test"] = [&]() {
            // Users API
            std::vector<User> users;
            DLOG << "Get Users" << std::endl;
            client->getUsers(users);
            for (auto u : users) {
                DLOG << u << std::endl;
            }
            DLOG << "Add more users" << std::endl;
            for (int i = 0; i < 10; i++) {
                User u;
                u.username = "ADD_USER_" + std::to_string(i);
                users.push_back(u);
            }
            client->setUsers(users);
            DLOG << "Get Users" << std::endl;
            client->getUsers(users);
            for (auto u : users) {
                DLOG << u << std::endl;
            }

            // Symbols API
            std::vector<Symbol> symbolsSrc;
            std::vector<Symbol> symbolsDst;
            DLOG << "Add symbols" << std::endl;
            for (int i = 0; i < 10; i++) {
                Symbol src;
                Symbol dst;

                auto f = [&](const std::string& t, Symbol& s) {
                    s.name = "Sym_" + t + "_" + std::to_string(i);
                    s.measure = "M_" + std::to_string(i);
                    for (int j = 0; j < 5; j++) {
                        s.tags.insert("Tag_" + std::to_string(i) + "_" + std::to_string(j));
                        s.kv["Key_" + std::to_string(j)] = "Val_" + std::to_string(j);
                    }
                };

                f("Src", src);
                f("Dst", dst);

                symbolsSrc.push_back(src);
                symbolsDst.push_back(dst);
            }
            client->setSymbols(symbolsSrc, symbolsDst);
            DLOG << "Get Symbols Src" << std::endl;
            symbolsSrc.clear();
            symbolsDst.clear();
            client->getSymbolsSrc(symbolsSrc);
            for (auto s : symbolsSrc) {
                DLOG << s << std::endl;
            }
            DLOG << "Get Symbols Dst" << std::endl;
            client->getSymbolsDst(symbolsDst);
            for (auto s : symbolsDst) {
                DLOG << s << std::endl;
            }
        };

        mapClientFn["db"] = [&]() {

            DLOGF << std::endl;
            THRIFT::TradeMachine tm;
            client->getTM(tm);
            DLOGF << std::endl;

            // MongoDB
            /*
            {
            mongocxx::instance inst{};
            mongocxx::client conn{mongocxx::uri{}};
            auto collection = conn["testdb"]["trademachine"];

            //DLOG << "TM ---------------- " << std::endl << tm << std::endl << "---------------------" << std::endl;

            bsoncxx::builder::stream::document document{};
            document << "id" << tm.spec.guid;
            document << "owner" << tm.spec.owner.username;
            collection.insert_one(document.view());

            DLOG << bsoncxx::to_json(document) << std::endl;
            }

            boost::shared_ptr<TMemoryBuffer> transportOut(new TMemoryBuffer());
            boost::shared_ptr<::apache::thrift::protocol::TJSONProtocol> protocolOut(new ::apache::thrift::protocol::TJSONProtocol(transportOut));
            tm.write(protocolOut.get());
            DLOG << transportOut->getBufferAsString() << std::endl;
*/

            DLOGF << std::endl;
            boost::shared_ptr<TRADE::TJSONProtocol> protoJson(new TRADE::TJSONProtocol());
            tm.write(protoJson.get());
            DLOG << std::endl << protoJson->getJson() << std::endl;


        };

        mapClientFn["read"] = [&]() {
            readTM(client);
        };

        mapClientFn["listen"] = [&]() {
            while (true) {
                monitor_.wait();
                readTM(client);
            }
        };

        mapClientFn["demoTrade"] = [&]() {
            TUsers users;
            client->getUsers(users);

            TSymbols symbolsSrc;
            client->getSymbolsSrc(symbolsSrc);
            TSymbols symbolsDst;
            client->getSymbolsDst(symbolsDst);

            for (int i = 0; i < count_->getValue(); i++) {
                int u = rand() % users.size();
                Order order;
                order.symbol = symbolsSrc[rand() % symbolsSrc.size()];
                order.symbolDst = symbolsDst[0];
                order.user = users[u];
                TRADE::Price::set(order.price, 1.3 + (double)(rand() % 1000) / 2000);
                TRADE::Volume::set(order.volume, 1 + (double)(rand() % 100) / 20);
                order.type = u % 2 == 0 ? THRIFT::OrderType::Sell : THRIFT::OrderType::Buy;
                client->postOrder(order);

                usleep(200000);
                //sleep(1);
            }

            isWorkerStopped = true;
            workerZmqSub.join();
        };

        // Call selected client func
        // DLOG << "Calling " << sub_->getValue() << std::endl;
        TRY(mapClientFn[sub_->getValue()]());
    };
}

void ModuleTrade::readTM(std::shared_ptr<SF::TRADE::THRIFT::BaseMachineClient> client) {
    std::vector<User> users;
    client->getUsers(users);
    DLOG << "Read " << users.size() << " Users" << std::endl;

    std::vector<Symbol> symbolsSrc;
    std::vector<Symbol> symbolsDst;
    client->getSymbolsSrc(symbolsSrc);
    client->getSymbolsDst(symbolsDst);
    DLOG << "Read " << symbolsSrc.size() << " Src Symbols" << std::endl;
    DLOG << "Read " << symbolsDst.size() << " Dst Symbols" << std::endl;

    THRIFT::OrderBook book;
    client->getOrderBook(book);
    TOrders& ordersHistory = book.orders["history"];
    DLOG << "Read " << ordersHistory.size() << " Orders" << std::endl;

    THRIFT::TradeMachine tm;
    client->getTM(tm);
    DLOG << "TM ---------------- " << std::endl << tm << std::endl << "---------------------" << std::endl;

}


} /* namespace SF */
