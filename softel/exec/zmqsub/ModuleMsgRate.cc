
#include <softel/core/common.h>

#include "ModuleMsgRate.hpp"

#include <softel/core/util/Options.h>
#include <softel/core/net/zmq/ZMQSubscription.h>
#include <softel/core/net/zmq/CGuardZMQMsg.hpp>

namespace CN = SF;


CN::ModuleMsgRate::ModuleMsgRate(const SF::CORE::UTIL::Options& opt)
    : ModuleBase(opt),
      host_(new SF::CORE::UTIL::OptionParam<std::string>("host", "localhost:21981"))
{
    setName("ModuleMsgRate");
    setInfo("Module meassuring stream speed aka message rate.");

    params_.push_back(host_);
    init();
}


CN::ModuleMsgRate::~ModuleMsgRate()
{
}

void CN::ModuleMsgRate::run()
{
    std::string url = "tcp://" + host_->getValue();

    ILOG << __FUNCTION__ << " url = " << url << std::endl << toString() << std::endl;

    SF::CORE::ZMQSubscription zmqSub(url);

    std::atomic<long> tickCount(0);

    std::atomic<long> tickCountDelta(0);
    std::atomic<long> tickRate(0);
    std::atomic<long> maxTickRate(0);
    int res = 1;
    int delay = res;

    boost::shared_ptr<boost::thread> t(new boost::thread([&]() {
        while (true) {
            sleep(1);

            tickRate = tickCountDelta / res;
            tickCountDelta = 0;

            if (maxTickRate < tickRate) {
                maxTickRate = tickRate.load();
            }

            ILOG << "MSG Rate: " << tickRate << " from " << tickCount << " max rate: " << maxTickRate << std::endl;
        }
    }));

    zmqSub.Receive([&](SF::CORE::CGuardZMQMsg& msg, const int size) {

        if (size == 0) {
            return;
        }

        ++tickCountDelta;

        ++tickCount;
    });

}
