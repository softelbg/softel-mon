/*
 * ThreadPoolAsio.h
 *
 *	Thread Pool using Boost Asio
 *
 *  Created on: 16.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef THREADPOOLASIO_H_
#define THREADPOOLASIO_H_

#include <boost/asio.hpp>
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/atomic.hpp>
#include <stack>

#include <softel/core/CObject.h>
#include "WorkItemBase.h"
#include <softel/core/lock/BoostSynchronized.h>
#include <softel/core/Ptr.h>

namespace SF {
namespace CORE {
namespace POOL {

/*
 *
 */
class ThreadPoolAsio: public SF::CORE::CObject {
public:
	ThreadPoolAsio(int size);
	virtual ~ThreadPoolAsio();

	void postWorkItem(SF::CORE::Ptr<WorkItemBase>  obj);

	void printStatus()  const {
		std::cout << "ThreadPool[" << m_size << "]";
		std::cout << " posted[" << m_posted << "]";
		std::cout << " completed[" << m_completed << "]";
		std::cout << " active[" << m_active << "]";
		std::cout << std::endl;
	}

	int getSize() const;
	int getPosted() const;
	int getActive() const;
	int getCompleted() const;

	static SF::CORE::LOCK::BMutex m_mutex;

	virtual std::string toString() const;
	virtual std::string toXml() const;

private:

	void threadfun(SF::CORE::Ptr<WorkItemBase> obj);

	boost::thread_group m_grp;
	boost::asio::io_service m_io_service;
	boost::asio::io_service::work m_work;

	boost::atomic<int> m_size;
	boost::atomic<int> m_posted;
	boost::atomic<int> m_completed;
	boost::atomic<int> m_active;

};

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */
#endif /* THREADPOOLASIO_H_ */
