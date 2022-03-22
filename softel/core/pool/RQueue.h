/*
 * RQueue.h
 *
 *	Thread safe Queue implementation using stl queue, but allow multuple
 *	threads to write/read the queue.
 *
 *  Created on: 30.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef RQUEUE_H_
#define RQUEUE_H_

#include <queue>
#include <softel/core/CObject.h>
#include <softel/core/lock/BoostSynchronized.h>

namespace SF {
namespace CORE {
namespace POOL {

template<class T>
class RQueue: public SF::CORE::CObject {
public:
	RQueue() {
	}
	virtual ~RQueue() {
	}

	void push(const T& value) {
		{
			SF::CORE::LOCK::WriteSynchronized lock(this->m_mutex);
			this->m_queue.push(value);
		}

		this->m_condition.notify_all();
	}

	T pop() {
		SF::CORE::LOCK::WriteSynchronized lock(this->m_mutex);

		while (this->m_queue.size() == 0) {
			this->m_condition.wait(lock);
		}

		T value = this->m_queue.front();
		this->m_queue.pop();

		return value;
	}

	T front() {
		SF::CORE::LOCK::ReadSynchronized lock(this->m_mutex);

		while (this->m_queue.size() == 0) {
			this->m_condition.wait(lock);
		}

		T value = this->m_queue.front();

		return value;
	}

	size_t size() {
		SF::CORE::LOCK::ReadSynchronized lock(this->m_mutex);
		return this->m_queue.size();
	}

	void clear() {
		SF::CORE::LOCK::WriteSynchronized lock(this->m_mutex);
		this->m_queue.clear();
	}

private:
	SF::CORE::LOCK::RWMutex m_mutex;
	SF::CORE::LOCK::RWCondition m_condition;

	std::queue<T> m_queue;
};

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */

#endif /* RQUEUE_H_ */
