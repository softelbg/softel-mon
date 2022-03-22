/*
 * CSchedulerMainAlert.cc
 *
 *  Created on: 02.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSchedulerMainAlert.h"
#include "CThresholdAlert.h"

namespace SF {
namespace OSS {
namespace EXEC {

CSchedulerMainAlert::CSchedulerMainAlert() {
	// TODO Auto-generated constructor stub

}

CSchedulerMainAlert::~CSchedulerMainAlert() {
	// TODO Auto-generated destructor stub
}

void CSchedulerMainAlert::run() {

	CThresholdAlert alertThreshold;

	alertThreshold.start();

	//alertThreshold.dumpInfo();

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */


