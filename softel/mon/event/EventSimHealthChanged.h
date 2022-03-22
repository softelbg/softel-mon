/*
 * EventSimHealthChanged.h
 *
 *  Created on: 02.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef EVENTSIMHEALTHCHANGED_H_
#define EVENTSIMHEALTHCHANGED_H_

#include "EventBase.h"

namespace SF {
namespace MON {
namespace EVENT {

/*
 *
 */
class EventSimHealthChanged: public SF::MON::EVENT::EventBase {
public:
	EventSimHealthChanged();
	EventSimHealthChanged(const std::string& name,
			const EventLevel& level);
	virtual ~EventSimHealthChanged();

	std::string toString();

};

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
#endif /* EVENTSIMHEALTHCHANGED_H_ */
