/*
 * ThreadPoolAsio.cc
 *
 *  Created on: 16.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "ThreadPoolAsio.h"
#include <sstream>

using namespace std;
using namespace boost;

namespace SF {
namespace CORE {
namespace POOL {

SF::CORE::LOCK::BMutex ThreadPoolAsio::ThreadPoolAsio::m_mutex;

ThreadPoolAsio::ThreadPoolAsio(int size) :
		m_work(m_io_service),
		m_size(size),
		m_posted(0),
		m_completed(0),
		m_active(0) {

	for (int i = 0; i < size; ++i) {
		m_grp.create_thread(boost::bind(&asio::io_service::run, &m_io_service));
	}

}

ThreadPoolAsio::~ThreadPoolAsio() {
	m_io_service.stop();
	m_grp.join_all();
}

void ThreadPoolAsio::postWorkItem(SF::CORE::Ptr<WorkItemBase> obj) {
	try {
		{
			SF::CORE::LOCK::BSynchronized lock(m_mutex);
			obj->setId(m_posted);
			std::cout << "Post WorkItem id " << obj->getId() << std::endl;
			m_posted++;
			m_active++;
		}

		m_io_service.post(boost::bind(&ThreadPoolAsio::threadfun, this, obj));
	} catch (...) {
		cout << __FILE__ << "::" << __FUNCTION__ << " Error" << endl;
	}

}

void ThreadPoolAsio::threadfun(SF::CORE::Ptr<WorkItemBase> obj) {

	obj->run();

	{
		SF::CORE::LOCK::BSynchronized lock(m_mutex);
		cout << "WorkItem Finished: " << obj->getId() << endl;
		m_completed++;
		m_active--;
	}

}

int ThreadPoolAsio::getSize() const {
	return this->m_size;
}

int ThreadPoolAsio::getPosted() const {
	return this->m_posted;
}

int ThreadPoolAsio::getActive() const {
	return this->m_active;
}

int ThreadPoolAsio::getCompleted() const {
	return this->m_completed;
}

string ThreadPoolAsio::toString() const {
	stringstream ss;

	ss << "type:" << typeid(this).name() << endl;
	ss << "size: " << m_size << endl;
	ss << "posted: " << m_posted << endl;
	ss << "completed: " << m_completed << endl;
	ss << "active: " << m_active << endl;

	return ss.str();
}

string ThreadPoolAsio::toXml() const {
	stringstream ss;

	ss << "<type>" << typeid(this).name() << "</type>" << endl;
	ss << "<size>" << m_size << "</size>" << endl;
	ss << "<posted>" << m_posted << "</posted>" << endl;
	ss << "<completed>" << m_completed << "</completed>" << endl;
	ss << "<active>" << m_active << "</active>" << endl;

	return ss.str();
}

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */
