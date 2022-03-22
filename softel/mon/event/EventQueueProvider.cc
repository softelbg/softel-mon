/*
 * EventQueueProvider.cc
 *
 *  Created on: 31.10.2013
 *      Author: stan
 */

#include "EventQueueProvider.h"

using namespace std;

namespace SF {
namespace MON {
namespace EVENT {

EventQueueProvider::EventQueueProvider() :
	EventHandlerBase(ALLEVENTS) {

}

EventQueueProvider::~EventQueueProvider() {
	// TODO Auto-generated destructor stub
}

/**
 * Hook this event handler to listen for all events.
 */
void EventQueueProvider::init() {


}

/**
 * Save the incomming event to the vector
 */
void EventQueueProvider::onEvent(const SF::CORE::Ptr<EventBase>& event) {
	SF::CORE::LOCK::WriteSynchronized lock(this->m_mutex);
	//size_t pos = m_events.size();
	m_events.push_back(event);
}

size_t EventQueueProvider::size() {
	SF::CORE::LOCK::ReadSynchronized lock(this->m_mutex);
	return this->m_events.size();
}

void EventQueueProvider::clear() {
	SF::CORE::LOCK::WriteSynchronized lock(this->m_mutex);
	this->m_events.clear();
	this->m_idx.clear();
}

void EventQueueProvider::dumpInfo() {
	SF::CORE::LOCK::ReadSynchronized lock(this->m_mutex);

	cout << __FILE__ << "::" << __FUNCTION__ << " size: " << this->m_events.size() << endl;

	for(int i = 0; i < m_events.size(); i++) {
		cout << __FILE__ << "::" << __FUNCTION__ << " event[" << i << "] " << m_events[i]->toString() << endl;
	}

}

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
