#pragma once

#include "Options.h"
#include <softel/core/intf/IModule.hpp>

namespace SF {
namespace CORE {
namespace UTIL {


class ModuleBase : virtual public IModule
{
public:
    explicit ModuleBase(const Options& opt) : opt_(opt) {}
    virtual ~ModuleBase() {}

    void setName(const std::string& name) {
        name_ = name;
    }
    void setInfo(const std::string& info) {
        info_ = info;
    }

    const std::string& getName() const {
        return name_;
    }
    const std::string& getInfo() const {
        return info_;
    }

    const TParams& params() const {
        return params_;
    }

    std::string toString() const;

protected:

    void init();

    std::string name_;
    std::string info_;

    TParams params_;
    Options opt_;
};


} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

