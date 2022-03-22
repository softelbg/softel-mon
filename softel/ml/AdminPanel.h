/*
 * AdminPanel.h
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_ML_ADMINPANEL_H_
#define SOFTEL_WT_ML_ADMINPANEL_H_

#include <softel/core/common.h>
#include <softel/core/commonDefs.h>
#include <softel/wt/BaseMachine.h>

#include <Wt/WApplication>
#include <Wt/WContainerWidget>


namespace SF {
namespace WT {

class AdminPanel : public Wt::WApplication {
public:
    explicit AdminPanel(const Wt::WEnvironment& env);
    virtual ~AdminPanel();
};

class RNNWidget : public Wt::WContainerWidget, public UpdateSubscriber {
public:
    explicit RNNWidget(const std::string& name, Wt::WContainerWidget *parent = 0);
    virtual ~RNNWidget() noexcept (true);
    
private:
    std::string name_;
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_ML_ADMINPANEL_H_ */
