/*
 * CSchedulerMainAlert.h
 *
 *  Created on: 02.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSCHEDULERMAINALERT_H_
#define CSCHEDULERMAINALERT_H_

#include <softel/core/util/CScheduler.h>

namespace SF {
namespace OSS {
namespace EXEC {

class CSchedulerMainAlert: public SF::CORE::UTIL::CScheduler {
public:
	CSchedulerMainAlert();
	virtual ~CSchedulerMainAlert();

	void run();

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CSCHEDULERMAINALERT_H_ */
