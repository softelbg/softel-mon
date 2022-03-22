#pragma once

#include <softel/core/util/OptionParam.hpp>

namespace SF {
namespace CORE {

typedef std::vector<std::shared_ptr<SF::CORE::UTIL::IParam> > TParams;

class IModule
{
public:
    virtual ~IModule() {}

    virtual void run() = 0;

    virtual const std::string& getName() const = 0;
    virtual const std::string& getInfo() const = 0;
    virtual const TParams& params() const = 0;
};

} /* namespace CORE */
} /* namespace SF */