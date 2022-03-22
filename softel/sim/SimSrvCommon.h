/*
 * SimSrvCommon.h
 *
 *  Various SIM-related (anonymous) subclasses
 *
 *  Created on: 10.08.2013
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef SIMSRVCOMMON_H_
#define SIMSRVCOMMON_H_

#include <softel/core/pool/ScheduledItem.h>
#include <softel/core/pool/WorkItemSchedule.h>
#include <softel/sim/SimDbPool.h>
#include <softel/core/net/web/BPluginBase.h>

namespace SF {
namespace SIM {


class DbScheduledItem : public SF::CORE::POOL::ScheduledItem {
	void run() {
		SimDbPool::getInstance()->refresh();
	}
};


} /* namespace SIM */
} /* namespace SF */

#endif /* SIMSRVCOMMON_H_ */
