/*
 * RQueueEventSingleton.cc
 *
 *  Created on: 02.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "RQueueEventSingleton.h"

using namespace SF::CORE::LOCK;
using namespace SF::CORE::POOL;

namespace SF {
namespace MON {
namespace EVENT {

RQueueEventSingleton* RQueueEventSingleton::m_pInstance = NULL;

RQueueEventSingleton::RQueueEventSingleton() {
}

RQueueEventSingleton::~RQueueEventSingleton() {
	// TODO Auto-generated destructor stub
}

RQueueEventSingleton* RQueueEventSingleton::getInstance() {

	SF::CORE::LOCK::BSynchronized lock(RQueueEventSingleton::m_mutex);

	if (RQueueEventSingleton::m_pInstance == 0) {
		RQueueEventSingleton::m_pInstance = new RQueueEventSingleton();
	}

	return RQueueEventSingleton::m_pInstance;
}

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
