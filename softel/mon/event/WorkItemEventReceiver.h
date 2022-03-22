/*
 * WorkItemEventReceiver.h
 *
 *  Created on: 15.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef WORKITEMEVENTRECEIVER_H_
#define WORKITEMEVENTRECEIVER_H_

#include <softel/core/pool/WorkItemBase.h>
#include "RQueueEvent.h"
#include "EventHandlerManager.h"

namespace SF {
namespace MON {
namespace EVENT {

/*
 *
 */
class WorkItemEventReceiver: public SF::CORE::POOL::WorkItemBase {
public:
	WorkItemEventReceiver(RQueueEvent& queue, EventHandlerManager& handlerManager);
	virtual ~WorkItemEventReceiver();

	void run();

private:
	RQueueEvent& m_event_queue;
	EventHandlerManager& m_handler_manager;
};

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
#endif /* WORKITEMEVENTRECEIVER_H_ */
