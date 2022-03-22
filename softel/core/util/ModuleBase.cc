#include <softel/core/common.h>
#include "ModuleBase.hpp"

namespace CN = SF::CORE::UTIL;

std::string CN::ModuleBase::toString() const
{
    std::stringstream ss;

    ss << name_ << " [" << info_ << "]" << std::endl;

    for (auto it = params_.begin(); it != params_.end(); ++it) {
        ss << "\t\t\t" << (*it)->toString() << std::endl;
    }

    return ss.str();
}

void CN::ModuleBase::init() {
    for (auto it = params_.begin(); it != params_.end(); ++it) {
        (*it)->init(opt_);
    }
}
