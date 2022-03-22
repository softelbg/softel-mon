
#include <softel/core/common.h>

#include "ModuleMsgMD.hpp"

#include <softel/core/util/Options.h>
#include <softel/trade/idl/messages.pb.h>
#include <softel/core/net/zmq/ZMQSubscription.h>
#include <softel/core/net/zmq/CGuardZMQMsg.hpp>

namespace CN = SF;


CN::ModuleMsgMD::ModuleMsgMD(const SF::CORE::UTIL::Options& opt)
    : ModuleBase(opt),
    host_(new SF::CORE::UTIL::OptionParam<std::string>("host", "localhost:21981"))
{
    setName("ModuleMsgMD");
    setInfo("Module reading Market Data structures.");

    params_.push_back(host_);
    init();
}


CN::ModuleMsgMD::~ModuleMsgMD()
{
}

void concurent_1()
{
    tbb::concurrent_unordered_map<long, std::string> m1;
    
    for (long i = 0; i < 10; i++) {
        m1[i] = "UM " + std::to_string(i);
    }
    
    auto it = m1.find(3);
    m1.unsafe_erase(it);
    
    m1.insert(std::pair<long, std::string>(33, "Insert UM 33"));
    
    for (it = m1.begin(); it != m1.end(); ++it) {
        ILOG << it->first << " -> " << it->second << std::endl;
    }
}

void concurent_2()
{
    tbb::concurrent_hash_map<long, std::string> m1;
    
    for (long i = 0; i < 10; i++) {
        m1.insert(std::pair<long, std::string>(i, "HM " + std::to_string(i)));
    }
    
    tbb::concurrent_hash_map<long, std::string>::accessor result;
    m1.find(result, 3);
    m1.erase(result);
    
    m1.insert(std::pair<long, std::string>(33, "Insert HM 33"));
    
    for (auto it = m1.begin(); it != m1.end(); ++it) {
        ILOG << it->first << " -> " << it->second << std::endl;
    }
}

void CN::ModuleMsgMD::run()
{
    std::string url = "tcp://" + host_->getValue();

    ILOG << __FUNCTION__ << " url = " << url << std::endl << toString() << std::endl;

    SF::CORE::ZMQSubscription zmqSub(url);

    size_t tickCount = 0;

    concurent_1();
    concurent_2();
    
    return;
    
    zmqSub.Receive([&](SF::CORE::CGuardZMQMsg& msg, const int size) {

        if (size == 0) {
            return;
        }

        SF::TRADE::MarketDataProto md;
        md.ParseFromArray(msg.Data(), msg.DataSize());
        
        ILOG << md.symbol() << " BID[" << md.bid() << "] ASK[" << md.ask() << "]" << std::endl;

    });
}
