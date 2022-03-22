/*
 * CSchedulerEnvMon.cc
 *
 *  Created on: 18.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSchedulerEnvMon.h"
#include "CMonDeviceHttpBlizoo.h"
#include <softel/core/net/CHost.h>

using namespace std;
using namespace SF::OSS::EXEC;
using namespace SF::CORE::NET;

namespace SF {
namespace OSS {
namespace EXEC {

CSchedulerEnvMon::CSchedulerEnvMon() {
	// TODO Auto-generated constructor stub

}

CSchedulerEnvMon::~CSchedulerEnvMon() {
	// TODO Auto-generated destructor stub
}

void CSchedulerEnvMon::run() {

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	/*
	CMonDeviceBlizoo device("192.168.2.53", 9760);
	device.connect();
	device.dumpInfo();
	*/

	CHost host("212.50.17.22", 80);

	CMonDeviceHttpBlizoo device(host);
	device.run();
	device.dumpInfo();

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */


