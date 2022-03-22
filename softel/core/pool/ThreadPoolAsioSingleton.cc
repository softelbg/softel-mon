/*
 * ThreadPoolAsioSingleton.cc
 *
 *  Created on: 14.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "ThreadPoolAsioSingleton.h"
#include <softel/core/commonDefs.h>

using namespace SF::CORE;
using namespace SF::CORE::LOCK;

namespace SF {
namespace CORE {
namespace POOL {

Ptr<ThreadPoolAsioSingleton> ThreadPoolAsioSingleton::m_pInstance;
BMutex	ThreadPoolAsioSingleton::m_mutex;

ThreadPoolAsioSingleton::ThreadPoolAsioSingleton(size_t size) :
	ThreadPoolAsio(size) {

}

ThreadPoolAsioSingleton::~ThreadPoolAsioSingleton() {
	// TODO Auto-generated destructor stub
}

void ThreadPoolAsioSingleton::init(size_t size) {

	BSynchronized lock(ThreadPoolAsioSingleton::m_mutex);

	if (!ThreadPoolAsioSingleton::m_pInstance) {
		ThreadPoolAsioSingleton::m_pInstance.reset(new ThreadPoolAsioSingleton(size));
	}

}

SF::CORE::Ptr<ThreadPoolAsioSingleton> ThreadPoolAsioSingleton::getInstance() {

	BSynchronized lock(ThreadPoolAsioSingleton::m_mutex);

	assert(ThreadPoolAsioSingleton::m_pInstance);

	return ThreadPoolAsioSingleton::m_pInstance;
}

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */
