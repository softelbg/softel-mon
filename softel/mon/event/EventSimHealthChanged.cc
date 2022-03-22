/*
 * EventSimHealthChanged.cc
 *
 *  Created on: 02.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include <sstream>
#include "EventSimHealthChanged.h"

namespace SF {
namespace MON {
namespace EVENT {

EventSimHealthChanged::EventSimHealthChanged() {
	// TODO Auto-generated constructor stub

}

EventSimHealthChanged::EventSimHealthChanged(const std::string& name,
		const EventLevel& level) :
		EventBase(name, level, "EventSimHealthChanged") {

}

EventSimHealthChanged::~EventSimHealthChanged() {
	// TODO Auto-generated destructor stub
}

std::string EventSimHealthChanged::toString() {
	std::stringstream ss;

	ss << "EventSimHealthChanged: name:" << this->getName();
	ss << " level: " << this->getLevel();

	return ss.str();
}

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
