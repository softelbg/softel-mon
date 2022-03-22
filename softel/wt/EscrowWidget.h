/*
 * EscrowApplication.h
 *
 *  Created on: Feb 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef ESCROWAPPLICATION_H_
#define ESCROWAPPLICATION_H_

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include "MongoTable.h"

namespace SF {
namespace WT {

class EscrowWidget : public Wt::WContainerWidget {
public:
	explicit EscrowWidget(Wt::WContainerWidget *parent = 0);
	virtual ~EscrowWidget();

private:
	void init();
	void initTable();

	MongoTable* table_;
};

} /* namespace WT */
} /* namespace SF */
#endif /* ESCROWAPPLICATION_H_ */
