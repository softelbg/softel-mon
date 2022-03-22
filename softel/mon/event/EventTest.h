/*
 * EventTest.h
 *
 *  Created on: 14.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef EVENTTEST_H_
#define EVENTTEST_H_

#include "EventBase.h"

namespace SF {
namespace MON {
namespace EVENT {

/*
 *
 */
class EventTest: public SF::MON::EVENT::EventBase {
public:
	EventTest(const std::string& name,
			const EventLevel& level);
	virtual ~EventTest();

};

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
#endif /* EVENTTEST_H_ */
