/*
 * WorkItemEventHandler.h
 *
 *	WorkItem which should process EventHandlers
 *
 *
 *  Created on: 15.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef WORKITEMEVENTHANDLER_H_
#define WORKITEMEVENTHANDLER_H_

#include <softel/core/commonDefs.h>
#include <softel/core/pool/WorkItemBase.h>
#include "EventHandlerBase.h"
#include "EventBase.h"

namespace SF {
namespace MON {
namespace EVENT {

/*
 *
 */
class WorkItemEventHandler: public SF::CORE::POOL::WorkItemBase {
public:
	WorkItemEventHandler(const SF::CORE::Ptr<EventHandlerBase>& eventHandler,
						const SF::CORE::Ptr<EventBase>& event);
	virtual ~WorkItemEventHandler();

	virtual void run();

private:
	SF::CORE::Ptr<EventHandlerBase> m_eventHandler;
	SF::CORE::Ptr<EventBase> m_event;

};

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
#endif /* WORKITEMEVENTHANDLER_H_ */
