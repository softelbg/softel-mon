/*
 * EventHandlerTest.h
 *
 *	Test EventHandler for prove of concept..
 *
 *  Created on: 15.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef EVENTHANDLERTEST_H_
#define EVENTHANDLERTEST_H_

#include "EventHandlerBase.h"

namespace SF {
namespace MON {
namespace EVENT {

/*
 *
 */
class EventHandlerTest: public SF::MON::EVENT::EventHandlerBase {
public:
	EventHandlerTest(int i);
	virtual ~EventHandlerTest();

	virtual void onEvent(const SF::CORE::Ptr<EventBase>& event);

private:
	int m_index;
};

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
#endif /* EVENTHANDLERTEST_H_ */
