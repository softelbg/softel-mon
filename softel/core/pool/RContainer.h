/*
 * RContainer.h
 *
 *	Thread safe Vector Container implementation using stl vector, but allow multuple
 *	threads to write/read the vector.
 *
 *  Created on: 30.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef RContainer_H_
#define RContainer_H_

#include <queue>
#include <softel/core/CObject.h>
#include <softel/core/lock/BoostSynchronized.h>
#include <softel/db/CPosIndex.h>

namespace SF {
namespace CORE {
namespace POOL {

template<class T>
class RContainer: public SF::CORE::CObject {
public:
	RContainer() {
	}
	virtual ~RContainer() {
	}

	void push_back(const T& value) {
		{
			SF::CORE::LOCK::WriteSynchronized lock(this->m_mutex);
			this->m_container.push_back(value);
		}

		this->m_condition.notify_all();
	}

	void push_back(const std::string& key, const T& value) {
		{
			SF::CORE::LOCK::WriteSynchronized lock(this->m_mutex);
			m_idx.setIdx(key, m_container.size());
			m_container.push_back(value);
		}

		this->m_condition.notify_all();
	}

	bool get(size_t pos, T& value) {
		SF::CORE::LOCK::ReadSynchronized lock(this->m_mutex);
		return getInternal(pos, value);
	}

	bool get(const std::string& key, T& value) {
		size_t pos = -1;

		SF::CORE::LOCK::ReadSynchronized lock(this->m_mutex);
		pos = m_idx.getIdx(key);
		return getInternal(pos, value);
	}

	T back() {
		SF::CORE::LOCK::ReadSynchronized lock(this->m_mutex);

		while (this->m_queue.size() == 0) {
			this->m_condition.wait(lock);
		}

		T value = this->m_queue.back();

		return value;
	}

	size_t size() {
		SF::CORE::LOCK::ReadSynchronized lock(this->m_mutex);
		return this->m_container.size();
	}

	void clear() {
		SF::CORE::LOCK::WriteSynchronized lock(this->m_mutex);
		this->m_container.clear();
	}

private:
	bool validatePos(size_t pos) {
		// Assert that we have already read lock here...
		return (pos >= 0 && pos < m_container.size());
	}

	bool getInternal(size_t pos, T& value) {
		if (validatePos(pos)) {
			value = m_container[pos];
			return true;
		} else {
			return false;
		}
	}


	SF::CORE::LOCK::RWMutex m_mutex;
	SF::CORE::LOCK::RWCondition m_condition;

	std::vector<T> m_container;
	SF::DB::CPosIndex m_idx;
};

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */

#endif /* RContainer_H_ */
