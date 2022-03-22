/*
 * ModuleMsgRaw.hpp
 *
 *  Created on: Apr 22, 2015
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2015 Softel ltd Haskovo. All rights reserved.
 *
 */
#pragma once

#include <softel/core/util/ModuleBase.hpp>

namespace SF {


class ModuleMsgRaw : public SF::CORE::UTIL::ModuleBase
{
public:
    explicit ModuleMsgRaw(const SF::CORE::UTIL::Options& opt);
    virtual ~ModuleMsgRaw();

    void run();

private:
    std::shared_ptr<SF::CORE::UTIL::OptionParam<std::string> > host_;
    std::shared_ptr<SF::CORE::UTIL::OptionParam<size_t> > ticksMaxCount_;
    std::shared_ptr<SF::CORE::UTIL::OptionParam<std::string> > continuous_;
};


}
