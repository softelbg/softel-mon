/*
 * EventManager.cc
 *
 *  Created on: 14.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "EventManager.h"
#include "WorkItemEventReceiver.h"
#include <softel/core/commonDefs.h>

using namespace SF::CORE;
using namespace SF::CORE::POOL;
using namespace std;

namespace SF {
namespace MON {
namespace EVENT {

EventManager::~EventManager() {
	// TODO Auto-generated destructor stub
}

EventManager::EventManager() {
	// TODO Auto-generated constructor stub

}

/**
 * Initialize the Event Manager.
 * Should start event receiver work items...
 *
 */
void EventManager::init() {

	Ptr<ThreadPoolAsioSingleton> pThrPool = ThreadPoolAsioSingleton::getInstance();

	m_handler_manager.init();

	for(int i = 0; i < 10; ++i) {
		SF::CORE::Ptr<WorkItemBase> ptrWIR(
			new WorkItemEventReceiver(m_event_queue, m_handler_manager));
		pThrPool->postWorkItem(ptrWIR);
	}

	// hook all events queue handler to collect all incomming events
	SF::CORE::Ptr<SF::MON::EVENT::EventHandlerBase> eventHandlerAll(&m_queue_all_events);
	addHandlerAllEvents(eventHandlerAll);
}

/**
 * Add an Event to the event queue - sync is done in the queue.
 */
void EventManager::addEvent(const SF::CORE::Ptr<EventBase>& event) {
	this->m_event_queue.push(event);
}

/**
 * Add new Event Handler to the event handler manager.
 */
void EventManager::addHandler(
		const SF::CORE::Ptr<EventHandlerBase>& eventHandler) {
	this->m_handler_manager.addHandler(eventHandler);
}

void EventManager::addHandlerAllEvents(
		const SF::CORE::Ptr<EventHandlerBase>& eventHandler) {
	this->m_handler_manager.addHandlerAllEvents(eventHandler);
}

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
