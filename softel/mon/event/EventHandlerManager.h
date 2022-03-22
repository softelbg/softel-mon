/*
 * EventHandlerManager.h
 *
 *	Manager for Event Handlers.
 *	Add new handlers to listen some or all events.
 *
 *  Created on: 14.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef EVENTHANDLERMANAGER_H_
#define EVENTHANDLERMANAGER_H_

#include <unordered_map>
#include <softel/core/commonDefs.h>
#include <softel/core/ManagerBase.h>
#include "EventHandlerBase.h"
#include "EventBase.h"

namespace SF {
namespace MON {
namespace EVENT {

/*
 *
 */
class EventHandlerManager: public SF::CORE::ManagerBase {
public:
	EventHandlerManager();
	virtual ~EventHandlerManager();

	void init();

	void addHandler(const SF::CORE::Ptr<EventHandlerBase>& eventHandler);
	void addHandlerAllEvents(const SF::CORE::Ptr<EventHandlerBase>& eventHandler);

	void pushEvent(const SF::CORE::Ptr<EventBase> event);

	void dumpInfo();

protected:

	void callHandlers(const SF::CORE::Ptr<EventBase> event,
			std::vector<SF::CORE::Ptr<EventHandlerBase> >& vectorHandlers);

	std::unordered_map<std::string, std::vector<SF::CORE::Ptr<EventHandlerBase> > > m_handlers;

	SF::CORE::LOCK::BMutex m_mutex; // Consider THIS Lockable object instead...
	SF::CORE::LOCK::BMutex m_mutex_events;
};

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
#endif /* EVENTHANDLERMANAGER_H_ */
