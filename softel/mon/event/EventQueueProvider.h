/*
 * EventQueueProvider.h
 *
 * Provides Event Queue which should listen for all incomming events and store them
 *
 * Should implement an effective event search methods.
 * For example search events by time period (from - to dates)
 * Or some more complex searches...
 *
 *  Created on: 31.10.2013
 *      Author: stan
 */

#ifndef EVENTQUEUEPROVIDER_H_
#define EVENTQUEUEPROVIDER_H_

#include <vector>
#include "EventHandlerBase.h"
#include <softel/db/CPosIndex.h>
#include <softel/core/commonDefs.h>

namespace SF {
namespace MON {
namespace EVENT {

class EventQueueProvider: public EventHandlerBase {
public:
	EventQueueProvider();
	virtual ~EventQueueProvider();

	void init();

	virtual void onEvent(const SF::CORE::Ptr<EventBase>& event);

	size_t size();
	void clear();

	void dumpInfo();

private:
	std::vector<SF::CORE::Ptr<EventBase> > m_events;
	SF::DB::CPosIndex m_idx;

	SF::CORE::LOCK::RWMutex m_mutex;
};

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
#endif /* EVENTQUEUEPROVIDER_H_ */
