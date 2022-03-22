/*
 * ThreadPoolAsioSingleton.h
 *
 *	Thread Pool Singleton to be used as a Main App Thread Pool
 *
 *  Created on: 14.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef THREADPOOLASIOSINGLETON_H_
#define THREADPOOLASIOSINGLETON_H_

#include "ThreadPoolAsio.h"
#include <softel/core/commonDefs.h>

namespace SF {
namespace CORE {
namespace POOL {

/*
 *
 */
class ThreadPoolAsioSingleton: public SF::CORE::POOL::ThreadPoolAsio {
public:
	virtual ~ThreadPoolAsioSingleton();

	static void init(size_t size);
	static SF::CORE::Ptr<ThreadPoolAsioSingleton> getInstance();

private:
	ThreadPoolAsioSingleton(size_t size);

	static SF::CORE::Ptr<ThreadPoolAsioSingleton> m_pInstance;
	static SF::CORE::LOCK::BMutex	m_mutex;
};

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */
#endif /* THREADPOOLASIOSINGLETON_H_ */
