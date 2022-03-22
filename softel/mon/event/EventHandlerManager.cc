/*
 * EventHandlerManager.cc
 *
 *  Created on: 14.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "EventHandlerManager.h"

using namespace std;

namespace SF {
namespace MON {
namespace EVENT {

EventHandlerManager::EventHandlerManager() {
	// TODO Auto-generated constructor stub

}

EventHandlerManager::~EventHandlerManager() {
	// TODO Auto-generated destructor stub
}

/**
 * Initialize the EventHandler Manager.
 *
 */
void EventHandlerManager::init() {
}

/**
 * Add an event handler for its specific event type only
 */
void EventHandlerManager::addHandler(
		const SF::CORE::Ptr<EventHandlerBase>& eventHandler) {

	SF::CORE::LOCK::BSynchronized lock(this->m_mutex);

	std::vector<SF::CORE::Ptr<EventHandlerBase> >& vectorHandlers =
			this->m_handlers[eventHandler->getEventType()];

	vectorHandlers.push_back(eventHandler);
}

/**
 * Add a handler who will receive all possible events
 */
void EventHandlerManager::addHandlerAllEvents(
		const SF::CORE::Ptr<EventHandlerBase>& eventHandler) {

	SF::CORE::LOCK::BSynchronized lock(this->m_mutex);

	std::vector<SF::CORE::Ptr<EventHandlerBase> >& vectorHandlers =
			this->m_handlers[ALLEVENTS];

	vectorHandlers.push_back(eventHandler);
}

/**
 * Push an event for further processing...
 *
 */
void EventHandlerManager::pushEvent(const SF::CORE::Ptr<EventBase> event) {

	SF::CORE::LOCK::BSynchronized lock(this->m_mutex);

	std::vector<SF::CORE::Ptr<EventHandlerBase> >& vectorHandlers = this->m_handlers[event->getType()];
	callHandlers(event, vectorHandlers);

	std::vector<SF::CORE::Ptr<EventHandlerBase> >& vectorHandlersAllEvents = this->m_handlers[ALLEVENTS];
	callHandlers(event, vectorHandlersAllEvents);
}

/**
 * Call onEvent for the list of event handlers
 * Asserts that the manager is locked...
 */
void EventHandlerManager::callHandlers(const SF::CORE::Ptr<EventBase> event,
		std::vector<SF::CORE::Ptr<EventHandlerBase> >& vectorHandlers) {
	for (int i = 0; i < vectorHandlers.size(); ++i) {
		vectorHandlers[i]->onEvent(event);
	}
}

void EventHandlerManager::dumpInfo() {
	//SF::CORE::LOCK::BSynchronized lock(this->m_mutex);

	cout << __FILE__ << "::" << __FUNCTION__ << " sizeHandlerTypes: " << this->m_handlers.size() << endl;
	std::unordered_map<std::string, std::vector<SF::CORE::Ptr<EventHandlerBase> > >::iterator it;
	for(it = this->m_handlers.begin(); it != m_handlers.end(); it++) {
		cout << __FILE__ << "::" << __FUNCTION__ << " handler:type: " << it->first;
		cout << " size: " << it->second.size() << endl;
	}
}

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
