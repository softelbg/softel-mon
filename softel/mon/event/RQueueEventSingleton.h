/*
 * RQueueEventSingleton.h
 *
 *  Created on: 02.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef RQUEUEEVENTSINGLETON_H_
#define RQUEUEEVENTSINGLETON_H_

#include "RQueueEvent.h"
#include <softel/core/lock/BoostSynchronized.h>

namespace SF {
namespace MON {
namespace EVENT {

/*
 *
 */
class RQueueEventSingleton: public RQueueEvent {
public:
	virtual ~RQueueEventSingleton();

	static RQueueEventSingleton* getInstance();

private:
	RQueueEventSingleton();

	static RQueueEventSingleton* m_pInstance;
	static SF::CORE::LOCK::BMutex	m_mutex;
};

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
#endif /* RQUEUEEVENTSINGLETON_H_ */
