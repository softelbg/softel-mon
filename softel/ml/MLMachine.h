/*
 * MLMachine.h
 *
 *  Created on: Mar 7, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_MLMACHINE_H_
#define SOFTEL_WT_MLMACHINE_H_

#include <softel/wt/BaseMachine.h>
#include <softel/core/common.h>
#include <deque>

namespace SF {
namespace ML {


class MLMachine : public SF::WT::BaseMachine {
public:
    MLMachine();
    virtual ~MLMachine() noexcept (false);

    virtual void init();
    
    std::deque<std::string> getMsg(const std::string& name) const {
        SF::CORE::LOCK::ReadSynchronized lock(rw_msg_);
        DLOGF << "name = " << name << " msg_ size = " << msg_.size() << std::endl;
        auto it = msg_.find(name);
        assert(it != msg_.end());
        return it->second;
    }
    
    std::vector<std::string> getNames() const {
        SF::CORE::LOCK::ReadSynchronized lock(rw_msg_);
        return names_;
    }

private:
    void process(const std::string& name, std::string& last_line);
    
    mutable SF::CORE::LOCK::RWMutex rw_msg_;
    std::map<std::string, std::deque<std::string> > msg_;
    std::vector<std::string> names_;
};

struct Machines {
    static std::shared_ptr<MLMachine> m;
};


} /* namespace ML */
} /* namespace SF */

#endif /* SOFTEL_WT_MLMACHINE_H_ */
