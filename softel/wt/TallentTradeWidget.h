/*
 * TallentTradeWidget.h
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_TALLENTTRADEWIDGET_H_
#define SOFTEL_WT_TALLENTTRADEWIDGET_H_

#include "BaseTradeWidget.h"

namespace SF {
namespace WT {

class TallentTradeWidget : public BaseTradeWidget {
public:
    explicit TallentTradeWidget(Wt::WContainerWidget *parent = 0);
    virtual ~TallentTradeWidget();

private:
    void start();
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_TALLENTTRADEWIDGET_H_ */
