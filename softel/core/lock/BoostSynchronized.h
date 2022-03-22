/*
 * BoostSynchronized.h
 *
 *	Boost scoped lock RAII wrapper - imitate java Synchronized
 *
 *  Created on: 26.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BOOSTSYNCHRONIZED_H_
#define BOOSTSYNCHRONIZED_H_

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/atomic.hpp>

#include <mutex>
#include <condition_variable>
#include <thread>

#include "StackAllocated.h"

namespace SF {
namespace CORE {
namespace LOCK {

/*
 * Typedef-ing some of the Boost-ers...
 */
typedef boost::mutex BMutex;
typedef boost::mutex::scoped_lock BSynchronized;
typedef boost::condition_variable BCondition;
typedef boost::shared_mutex RWMutex;
typedef boost::condition_variable_any RWCondition;
typedef boost::shared_lock<boost::shared_mutex> ReadSynchronized;
typedef boost::unique_lock<boost::shared_mutex> WriteSynchronized;

// STD
typedef std::mutex Mutex;
typedef std::condition_variable Condition;
typedef std::unique_lock<std::mutex> Synchronized;


} /* namespace LOCK */
} /* namespace CORE */
} /* namespace SF */
#endif /* BOOSTSYNCHRONIZED_H_ */
