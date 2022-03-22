/*
 * BillingWidget.h
 *
 *  Created on: Mar 15, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_ADMTRADE_BILLINGWIDGET_H_
#define SOFTEL_WT_ADMTRADE_BILLINGWIDGET_H_

#include <Wt/WContainerWidget>
#include <softel/wt/BaseMachine.h>

namespace SF {
namespace WT {

class BillingWidget : public Wt::WContainerWidget, public UpdateSubscriber {
public:
    explicit BillingWidget(Wt::WContainerWidget *parent = 0);
    virtual ~BillingWidget();

private:
    void start();
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_ADMTRADE_BILLINGWIDGET_H_ */
