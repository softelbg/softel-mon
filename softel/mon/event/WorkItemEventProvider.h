/*
 * WorkItemEventProvider.h
 *
 *  Created on: 02.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef WORKITEMEVENTPROVIDER_H_
#define WORKITEMEVENTPROVIDER_H_

#include <softel/core/pool/WorkItemBase.h>
#include "EventManager.h"

namespace SF {
namespace MON {
namespace EVENT {


/*
 *
 */
class WorkItemEventProvider: public SF::CORE::POOL::WorkItemBase {
public:
	WorkItemEventProvider(int waitPeriod, SF::MON::EVENT::EventManager& eventManager);
	virtual ~WorkItemEventProvider();

	virtual void run();

private:
	int m_waitPeriod;

	SF::MON::EVENT::EventManager& m_event_manager;

};

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */

#endif /* WORKITEMEVENTPROVIDER_H_ */
