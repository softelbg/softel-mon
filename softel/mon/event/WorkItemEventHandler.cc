/*
 * WorkItemEventHandler.cc
 *
 *  Created on: 15.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "WorkItemEventHandler.h"

namespace SF {
namespace MON {
namespace EVENT {

WorkItemEventHandler::WorkItemEventHandler(const SF::CORE::Ptr<EventHandlerBase>& eventHandler,
		const SF::CORE::Ptr<EventBase>& event) {

	m_eventHandler = eventHandler;
	m_event = event;
}

WorkItemEventHandler::~WorkItemEventHandler() {
	// TODO Auto-generated destructor stub
}

void WorkItemEventHandler::run() {
}

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
