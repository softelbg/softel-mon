/*
 * WorkItemEventReceiver.cc
 *
 *  Created on: 15.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "WorkItemEventReceiver.h"
#include <softel/core/Ptr.h>

namespace SF {
namespace MON {
namespace EVENT {

WorkItemEventReceiver::WorkItemEventReceiver(RQueueEvent& queue,
		EventHandlerManager& handlerManager)
	: m_event_queue(queue), m_handler_manager(handlerManager){

}

WorkItemEventReceiver::~WorkItemEventReceiver() {
	// TODO Auto-generated destructor stub
}

void WorkItemEventReceiver::run() {

	while (true) {
		SF::CORE::Ptr<EventBase> event = m_event_queue.pop();

		std::cout << "WorkItem: " << this->getId() << " Received: "
				<< event->toString() << std::endl;

		this->m_handler_manager.pushEvent(event);
	}

}

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
