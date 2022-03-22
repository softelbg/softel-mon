/*
 * WorkItemEventProvider.cc
 *
 *  Created on: 02.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "WorkItemEventProvider.h"

#include <softel/core/commonDefs.h>
#include <softel/core/system/CSystemTools.h>
#include "EventSimHealthChanged.h"
#include "EventTest.h"
#include "EventBase.h"

using namespace SF::CORE::SYSTEM;
using namespace std;

namespace SF {
namespace MON {
namespace EVENT {

WorkItemEventProvider::WorkItemEventProvider(int waitPeriod,
		SF::MON::EVENT::EventManager& eventManager) :
		m_waitPeriod(waitPeriod), m_event_manager(eventManager) {

}

WorkItemEventProvider::~WorkItemEventProvider() {
	// TODO Auto-generated destructor stub
}

void WorkItemEventProvider::run() {

	while (true) {
		cout << "WorkItem: " << this->getId() << " Sleeping for "
				<< m_waitPeriod << endl;

		CSystemTools::doSleep(m_waitPeriod);

		stringstream ss;
		ss << "Test" << this->getId();

		SF::CORE::Ptr<EventBase> ptrEvent(new EventTest(ss.str(), EventLevel::WARN));
		m_event_manager.addEvent(ptrEvent);

		SF::CORE::Ptr<EventBase> ptrEvent2(new EventSimHealthChanged(ss.str(), EventLevel::ERROR));
		m_event_manager.addEvent(ptrEvent2);

	}

}

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */

