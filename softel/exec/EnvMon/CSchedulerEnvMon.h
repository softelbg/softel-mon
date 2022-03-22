/*
 * CSchedulerEnvMon.h
 *
 *  Created on: 18.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSCHEDULERENVMON_H_
#define CSCHEDULERENVMON_H_

#include <softel/core/util/CScheduler.h>

namespace SF {
namespace OSS {
namespace EXEC {

class CSchedulerEnvMon: public SF::CORE::UTIL::CScheduler {
public:
	CSchedulerEnvMon();
	virtual ~CSchedulerEnvMon();

	void run();

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CSCHEDULERENVMON_H_ */
