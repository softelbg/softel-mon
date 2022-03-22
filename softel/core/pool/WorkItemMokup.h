/*
 * WorkItemMokup.h
 *
 *  Created on: 16.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef WORKITEMMOKUP_H_
#define WORKITEMMOKUP_H_

#include "WorkItemBase.h"

namespace SF {
namespace CORE {
namespace POOL {

/*
 *
 */
class WorkItemMokup: public SF::CORE::POOL::WorkItemBase {
public:
	WorkItemMokup(int id);
	virtual ~WorkItemMokup();

	virtual void run();

};

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */
#endif /* WORKITEMMOKUP_H_ */
