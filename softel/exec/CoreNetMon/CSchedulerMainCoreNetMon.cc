/*
 * CSchedulerMainCoreNetMon.cc
 *
 *  Created on: 25.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSchedulerMainCoreNetMon.h"
#include <softel/oss/nms/CMonNetElemUps.h>

using namespace SF::OSS::NMS;
using namespace SF::OSS::EXEC;

namespace SF {
namespace OSS {
namespace EXEC {

CSchedulerMainCoreNetMon::CSchedulerMainCoreNetMon() {
	// TODO Auto-generated constructor stub

}

CSchedulerMainCoreNetMon::~CSchedulerMainCoreNetMon() {
	// TODO Auto-generated destructor stub
}

void CSchedulerMainCoreNetMon::run() {

	std::cout << __FILE__ << "::" << __FUNCTION__ << std::endl;

	CMonNetElemUps monUps;

	monUps.init();
	monUps.run();

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */


