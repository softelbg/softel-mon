/*
 * NewMachine.h
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_ADMTRADE_NEWMACHINE_H_
#define SOFTEL_WT_ADMTRADE_NEWMACHINE_H_

#include <softel/wt/BaseMachine.h>

namespace SF {
namespace WT {


class NewMachine : public BaseMachine {
public:
    NewMachine();
    virtual ~NewMachine();

    void go(SF::TRADE::THRIFT::TMSpec& m);
    void kill(const SF::TRADE::THRIFT::TMSpec& m);
    void killall();

    void getTypes(TTypes& types) const;
    std::string getType(size_t idx) const;

    void getMachines(TMachines& machines) const;
    void getMachines(size_t idxOwner, TMachines& machines) const;
    void getMachines(const SF::TRADE::THRIFT::User& user, TMachines& machines) const;
    void getMyMachines(TMachines& machines) const;

private:
    void initDemo();

    TTypes types_;

    TMachines machines_;
    TMachineIdx idxUser_;

    int port_;
    std::string host_;
};


// TODO: just to keep the single instances, make it better...
struct AdminMachines {
    // Admin machines
    static std::shared_ptr<NewMachine> admnewmachine;
};


} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_ADMTRADE_NEWMACHINE_H_ */
