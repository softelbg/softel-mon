#include <softel/core/common.h>

#include "ModulePublish.hpp"

#include <softel/core/util/Options.h>
#include <softel/core/net/zmq/ZMQPublish.h>
#include <softel/core/net/zmq/CGuardZMQMsg.hpp>
#include <softel/trade/idl/messages.pb.h>

#include <softel/core/util/SymbolMap.h>

namespace CN = SF;
using namespace SF::CORE;
using namespace SF::CORE::UTIL;

CN::ModulePublish::ModulePublish(const SF::CORE::UTIL::Options& opt)
    : ModuleBase(opt),
    delay_(new SF::CORE::UTIL::OptionParam<size_t>("delay", 10, "ms")),
    port_(new SF::CORE::UTIL::OptionParam<std::string>("port", "21981")),
    proto_(new SF::CORE::UTIL::OptionParam<std::string>("proto", "MDR")),
	path_(new SF::CORE::UTIL::OptionParam<std::string>("path", "/home/sgeorgiev/Develop/EUR_USD_Week4.csv"))
{
    setName("ModulePublish");
    setInfo("Module Publisher.");

    params_.push_back(delay_);
    params_.push_back(port_);
    params_.push_back(proto_);
    params_.push_back(path_);
    init();
}


CN::ModulePublish::~ModulePublish()
{
}

static const std::vector<std::string> symbols = {"EUR/USD", "EUR/XBT", "XBT/EUR"};

void CN::ModulePublish::run()
{
    std::string url = "tcp://*:" + port_->getValue();

    ILOG << __FUNCTION__ << " url = " << url << std::endl << toString();

    SF::CORE::ZMQPublish zmqPub(url, 1000);

    long long id = 0;

    std::map<std::string, std::function<void()> > mapFunc;

    srand (time(NULL));

    mapFunc["MDR"] = [&]()
    {
    	SF::TRADE::MarketDataProto md;
        int row = rand() % symbols.size();
        md.set_symbol(symbols[row]);
        md.set_ask(1.1 + (double)(rand() % 1000) / 10000);
        md.set_bid(md.ask() - (double)(rand() % 100) / 10000);
        
        // TODO: make proper function...
        size_t msgSize = md.ByteSize();
        CGuardZMQMsg zmqMsg(zmqPub.getHelper().get(), msgSize);
        md.SerializeToArray(zmqMsg.Data(), msgSize);
        zmqPub.Send(zmqMsg);

        if (delay_->isFound()) {
            usleep(delay_->getValue() * 1000);
        }
    };

    mapFunc["MDF"] = [&]()
    {
    	SymbolMap sm(path_->getValue());
    	sm.init();
    	const TableBase& table = sm.getTable();

    	ILOG << "table size = " << table.size() << std::endl;

    	TCol colSymbol;
    	TCol colAsk;
    	TCol colBid;
    	table.getColumn("CurrencyPair", colSymbol);
    	table.getColumn("RateAsk", colAsk);
    	table.getColumn("RateBid", colBid);

    	SF::TRADE::MarketDataProto md;

    	while (true) {
			for (size_t i = 0; i < table.size(); i++) {

				md.set_symbol(colSymbol[i]);
				md.set_ask(std::stod(colAsk[i]));
				md.set_bid(std::stod(colBid[i]));

				//ILOG << "Send: " << md.symbol() << " " << md.ask() << " " << md.bid() << std::endl;

				size_t msgSize = md.ByteSize();
				CGuardZMQMsg zmqMsg(zmqPub.getHelper().get(), msgSize);
				md.SerializeToArray(zmqMsg.Data(), msgSize);
				zmqPub.Send(zmqMsg);

				if (delay_->isFound()) {
					usleep(delay_->getValue() * 1000);
				}
			}
    	}
    };

    
    std::function<void()> fnSelected = mapFunc[proto_->getValue()];

    while (true) {
        fnSelected();
    }

}
