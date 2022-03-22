/*
 * EventHandlerTest.cc
 *
 *  Created on: 15.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "EventHandlerTest.h"

namespace SF {
namespace MON {
namespace EVENT {

EventHandlerTest::EventHandlerTest(int i) :
	EventHandlerBase("EventTest"),
	m_index(i) {

}

EventHandlerTest::~EventHandlerTest() {
	// TODO Auto-generated destructor stub
}

void EventHandlerTest::onEvent(const SF::CORE::Ptr<EventBase>& event) {
	std::cout << "EventHandlerTest::onEvent[" << m_index << "] "
			<< event->toString() << std::endl;

	if (event->getType().compare(this->getEventType()) == 0) {
		std::cout << "Event Type for the event and handler are correct." << std::endl;
	} else {
		std::cout << "Event Type Mismatch..." << std::endl;
	}
}

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
