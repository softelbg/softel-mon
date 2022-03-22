/*
 * CSchedulerCmAlert.h
 *
 *  Created on: 21.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSCHEDULERCMALERT_H_
#define CSCHEDULERCMALERT_H_

#include <softel/core/util/CScheduler.h>
#include <softel/core/system/CSystemTools.h>
#include "CCmtsCmAlertList.h"

namespace SF {
namespace OSS {
namespace EXEC {

class CSchedulerCmAlert : public SF::CORE::UTIL::CScheduler {

public:
	CSchedulerCmAlert();
	~CSchedulerCmAlert();

	void initCmList();

	void run();

private:

	CCmtsCmAlertList m_cmList;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */


#endif /* CSCHEDULERCMALERT_H_ */
