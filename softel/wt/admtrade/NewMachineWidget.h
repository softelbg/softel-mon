/*
 * NewMachineWidget.h
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_ADMTRADE_NEWMACHINEWIDGET_H_
#define SOFTEL_WT_ADMTRADE_NEWMACHINEWIDGET_H_

#include <Wt/WContainerWidget>
#include <softel/wt/BaseMachine.h>
#include "NewMachine.h"

namespace SF {
namespace WT {

class NewMachineWidget : public Wt::WContainerWidget, public UpdateSubscriber {
public:
    explicit NewMachineWidget(Wt::WContainerWidget *parent = 0);
    virtual ~NewMachineWidget();

private:
    void start();

    Wt::WTable *tableMachines_;
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_ADMTRADE_NEWMACHINEWIDGET_H_ */
