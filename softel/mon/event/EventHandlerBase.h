/*
 * EventHandlerBase.h
 *
 *	Base Event Handler which should be used for Event Listener and
 *	to be added to handlers list of the event handler manager to listen for specific events.
 *
 *  Created on: 14.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef EVENTHANDLERBASE_H_
#define EVENTHANDLERBASE_H_

#include <softel/core/commonDefs.h>
#include <softel/mon/core/HandlerBase.h>
#include "EventBase.h"

namespace SF {
namespace MON {
namespace EVENT {

#define ALLEVENTS	"allevents"

/*
 *
 */
class EventHandlerBase: public SF::MON::CORE::HandlerBase {
public:
	EventHandlerBase(const std::string& eventType);
	virtual ~EventHandlerBase();

	virtual void onEvent(const SF::CORE::Ptr<EventBase>& event) = 0;

	const std::string& getEventType() const {
		return this->m_event_type;
	}
private:
	std::string m_event_type;

};

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
#endif /* EVENTHANDLERBASE_H_ */
