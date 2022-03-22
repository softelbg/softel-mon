/*
 * OrderMarketWidget.h
 *
 *  Created on: Mar 6, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_ORDERMARKETWIDGET_H_
#define SOFTEL_WT_ORDERMARKETWIDGET_H_

#include "BaseOrderWidget.h"

namespace SF {
namespace WT {


class OrderMarketWidget : public BaseOrderWidget {
public:
	OrderMarketWidget(std::shared_ptr<TradeMachine> tradeMachine, Wt::WContainerWidget *parent = 0);
	virtual ~OrderMarketWidget();

	void update();
        void init();
private:
	void updateSymbol();
        void updateUserInfo();
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_ORDERMARKETWIDGET_H_ */
