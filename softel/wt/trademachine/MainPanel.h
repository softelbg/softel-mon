/*
 * MainPanel.h
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_TRADEMACHINE_MAINPANEL_H_
#define SOFTEL_WT_TRADEMACHINE_MAINPANEL_H_

#include <Wt/WApplication>
#include <softel/wt/BaseMachine.h>

namespace SF {
namespace WT {

class MainPanel : public Wt::WApplication {
public:
    explicit MainPanel(const Wt::WEnvironment& env);
    virtual ~MainPanel();
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_TRADEMACHINE_MAINPANEL_H_ */
