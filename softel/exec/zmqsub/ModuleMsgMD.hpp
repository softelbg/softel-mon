#pragma once
#include <softel/core/util/ModuleBase.hpp>

namespace SF
{


class ModuleMsgMD : public SF::CORE::UTIL::ModuleBase
{
public:
    explicit ModuleMsgMD(const SF::CORE::UTIL::Options& opt);
    virtual ~ModuleMsgMD();

    void run();

private:
    std::shared_ptr<SF::CORE::UTIL::OptionParam<std::string> > host_;
};


}