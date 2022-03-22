/*
 * CSchedulerMainCoreNetMon.h
 *
 *	Main Core Network Monitoring Scheduler - should execute core net mon
 *
 *  Created on: 25.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSCHEDULERMAINCORENETMON_H_
#define CSCHEDULERMAINCORENETMON_H_

#include <softel/core/util/CScheduler.h>

namespace SF {
namespace OSS {
namespace EXEC {

class CSchedulerMainCoreNetMon: public SF::CORE::UTIL::CScheduler {
public:
	CSchedulerMainCoreNetMon();
	virtual ~CSchedulerMainCoreNetMon();

	void run();

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CSCHEDULERMAINCORENETMON_H_ */
