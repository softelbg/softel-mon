/*
 * EventManager.h
 *
 *	Event Manager - should handle all events related stuff
 *	Some notifier/listener functionality.
 *	Add/Remove Events in a queue.
 *
 *	Input Events -> Event Queue -> Events Processor
 *								-> Event Handlers
 *								-> Event Persistence
 *								....
 *
 *  Created on: 14.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef MANAGEREVENT_H_
#define MANAGEREVENT_H_

#include "EventBase.h"
#include "EventHandlerManager.h"
#include "EventQueueProvider.h"
#include <softel/mon/event/RQueueEvent.h>
#include <softel/core/ManagerBase.h>
#include <softel/core/commonDefs.h>

namespace SF {
namespace MON {
namespace EVENT {

/*
 *
 */
class EventManager: public SF::CORE::ManagerBase {
public:
	EventManager();
	virtual ~EventManager();

	void init();

	void addEvent(const SF::CORE::Ptr<EventBase>& event);

	void addHandler(const SF::CORE::Ptr<EventHandlerBase>& eventHandler);
	void addHandlerAllEvents(const SF::CORE::Ptr<EventHandlerBase>& eventHandler);

	RQueueEvent& getEventQueue() {
		return m_event_queue;
	}

	EventHandlerManager& getEventHandlerManager() {
		return m_handler_manager;
	}

	EventQueueProvider& getEventQueueProvider() {
		return m_queue_all_events;
	}

	size_t sizeEventQueue() {
		return this->m_event_queue.size();
	}
private:

	RQueueEvent m_event_queue;
	EventHandlerManager m_handler_manager;

	EventQueueProvider m_queue_all_events;

};

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
#endif /* MANAGEREVENT_H_ */
