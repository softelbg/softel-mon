#pragma once

#include <softel/core/util/ModuleBase.hpp>

namespace SF
{


class ModulePublish : public SF::CORE::UTIL::ModuleBase
{
public:
public:
    explicit ModulePublish(const SF::CORE::UTIL::Options& opt);
    virtual ~ModulePublish();

    void run();

private:
    std::shared_ptr<SF::CORE::UTIL::OptionParam<std::string> > port_;
    std::shared_ptr<SF::CORE::UTIL::OptionParam<size_t> > delay_;
    std::shared_ptr<SF::CORE::UTIL::OptionParam<std::string> > proto_;
    std::shared_ptr<SF::CORE::UTIL::OptionParam<std::string> > path_;
};


}
