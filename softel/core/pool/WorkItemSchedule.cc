/*
 * WorkItemSchedule.cc
 *
 *  Created on: 27.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "WorkItemSchedule.h"
#include <softel/core/util/DateTime.h>
#include <softel/core/system/CSystemTools.h>

using namespace std;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace CORE {
namespace POOL {

#define PERIOD_SLEEP 60

WorkItemSchedule::WorkItemSchedule() {

}

WorkItemSchedule::~WorkItemSchedule() {
	// TODO Auto-generated destructor stub
}

void WorkItemSchedule::run() {

	while(true) {
		DateTime dt;

		for(size_t i = 0; i < m_scheduled_items.size(); i++) {
			if (dt.getTimeStamp() %  m_scheduled_items[i]->getPeriod() <= PERIOD_SLEEP) {
				m_scheduled_items[i]->run();
			}
		}

		SF::CORE::SYSTEM::CSystemTools::doSleep(PERIOD_SLEEP);
	}
}

void WorkItemSchedule::add(const SF::CORE::Ptr<ScheduledItem>& si) {
	m_scheduled_items.push_back(si);
}

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */

