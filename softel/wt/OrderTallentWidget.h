/*
 * OrderTallentWidget.h
 *
 *  Created on: Apr 13, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_ORDERTALLENTWIDGET_H_
#define SOFTEL_WT_ORDERTALLENTWIDGET_H_

#include "BaseOrderWidget.h"

namespace SF {
namespace WT {

class OrderTallentWidget : public BaseOrderWidget {
public:
    OrderTallentWidget(std::shared_ptr<TradeMachine> tradeMachine, Wt::WContainerWidget *parent = 0);
    virtual ~OrderTallentWidget();

private:
    void init();
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_ORDERTALLENTWIDGET_H_ */
