/*
 * WorkItemMokup.cc
 *
 *  Created on: 16.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include <boost/thread/mutex.hpp>

#include "WorkItemMokup.h"
#include "ThreadPoolAsio.h"
#include <softel/core/system/CSystemTools.h>

namespace SF {
namespace CORE {
namespace POOL {

WorkItemMokup::WorkItemMokup(int id) {
	this->setId(id);
}

WorkItemMokup::~WorkItemMokup() {
	// TODO Auto-generated destructor stub
}

void WorkItemMokup::run() {

	{
		boost::mutex::scoped_lock lock(ThreadPoolAsio::m_mutex);
		std::cout << "long task id " << getId() << " Started" << std::endl;
	}

	SF::CORE::SYSTEM::CSystemTools::doSleep(getId()*3 + 2);

	{
		boost::mutex::scoped_lock lock(ThreadPoolAsio::m_mutex);
		std::cout << "long task id " << getId() << " Ended" << std::endl;

		this->dumpInfo();
	}

}

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */
