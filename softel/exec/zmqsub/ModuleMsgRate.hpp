#pragma once
#include <softel/core/util/ModuleBase.hpp>

namespace SF
{


class ModuleMsgRate : public SF::CORE::UTIL::ModuleBase
{
public:
    explicit ModuleMsgRate(const SF::CORE::UTIL::Options& opt);
    virtual ~ModuleMsgRate();

    void run();

private:
    std::shared_ptr<SF::CORE::UTIL::OptionParam<std::string> > host_;
};


}
