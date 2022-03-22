/*
 * EventTest.cc
 *
 *  Created on: 14.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "EventTest.h"

namespace SF {
namespace MON {
namespace EVENT {

EventTest::EventTest(const std::string& name,
		const EventLevel& level) :
		EventBase(name, level, "EventTest") {
}

EventTest::~EventTest() {
	// TODO Auto-generated destructor stub
}

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
