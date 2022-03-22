/*
 * ModuleTrade.h
 *
 *  Created on: Apr 22, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_EXEC_ZMQSUB_MODULETRADE_H_
#define SOFTEL_EXEC_ZMQSUB_MODULETRADE_H_

#include <softel/trade/TradeMachine.h>
#include <softel/core/util/ModuleBase.hpp>

namespace SF {

class ModuleTrade : public SF::CORE::UTIL::ModuleBase {
public:
    explicit ModuleTrade(const SF::CORE::UTIL::Options& opt);
    virtual ~ModuleTrade();

    void run();

private:
    void initFnMap();

    void readTM(std::shared_ptr<SF::TRADE::THRIFT::BaseMachineClient> client);

    std::shared_ptr<SF::CORE::UTIL::OptionParam<int> > port_;
    std::shared_ptr<SF::CORE::UTIL::OptionParam<std::string> > mode_;
    std::shared_ptr<SF::CORE::UTIL::OptionParam<std::string> > proto_;
    std::shared_ptr<SF::CORE::UTIL::OptionParam<std::string> > sub_;
    std::shared_ptr<SF::CORE::UTIL::OptionParam<int> > count_;

    SF::TRADE::THRIFT::User owner_;
    std::map<std::string, std::function<void()>> mapFn_;

    mutable SF::CORE::LOCK::Monitor monitor_;
};

} /* namespace SF */

#endif /* SOFTEL_EXEC_ZMQSUB_MODULETRADE_H_ */
