/*
 * EventHandlerBase.cc
 *
 *  Created on: 14.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "EventHandlerBase.h"

namespace SF {
namespace MON {
namespace EVENT {

EventHandlerBase::EventHandlerBase(const std::string& eventType) :
	m_event_type(eventType) {

}

EventHandlerBase::~EventHandlerBase() {
	// TODO Auto-generated destructor stub
}

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
