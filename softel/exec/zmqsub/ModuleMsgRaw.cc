/*
 * ModuleMsgRaw.cc
 *
 *  Created on: Apr 22, 2015
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2015 Softel ltd Haskovo. All rights reserved.
 *
 */

#include <softel/core/common.h>
#include <softel/core/Helper.h>
#include <softel/core/util/Options.h>
#include <softel/core/net/zmq/ZMQSubscription.h>
#include <softel/core/net/zmq/CGuardZMQMsg.hpp>

#include <softel/exec/zmqsub/ModuleMsgRaw.hpp>

namespace SF {


ModuleMsgRaw::ModuleMsgRaw(const SF::CORE::UTIL::Options& opt)
    : ModuleBase(opt),
    host_(new SF::CORE::UTIL::OptionParam<std::string>("host", "localhost:21981")),
    ticksMaxCount_(new SF::CORE::UTIL::OptionParam<size_t>("ticksMaxCount", 100, "num")),
    continuous_(new SF::CORE::UTIL::OptionParam<std::string>("cont"))
{
    setName("ModuleMsgRaw");
    setInfo("Module reading raw zmq data.");

    params_.push_back(host_);
    params_.push_back(ticksMaxCount_);
    params_.push_back(continuous_);
    init();
}

ModuleMsgRaw::~ModuleMsgRaw()
{
}

void ModuleMsgRaw::run()
{
    std::string url = "tcp://" + host_->getValue();

    ILOG << __FUNCTION__ << " url = " << url << std::endl << toString();

    SF::CORE::ZMQSubscription zmqSub(url);

    size_t tickCount = 0;

    zmqSub.Receive([&](SF::CORE::CGuardZMQMsg& msg, const int size) {
        if (size == 0) {
            return;
        }

        ILOG << "size = " << size;
        ILOG << SF::CORE::Helper::toHex((unsigned char*)msg.Data(), size);
    });
}


}
