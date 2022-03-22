/*
 * CThrChildWaiter.cc
 *
 *  Created on: 07.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CThrChildWaiter.h"
#include <softel/core/system/CSystemTools.h>

using namespace std;
using namespace SF::CORE::SYSTEM;


namespace SF {
namespace OSS {
namespace EXEC {

CThrChildWaiter::CThrChildWaiter() {
	// TODO Auto-generated constructor stub

}

CThrChildWaiter::~CThrChildWaiter() {
	// TODO Auto-generated destructor stub
}

int CThrChildWaiter::run() {

	while(true) {

		int status;

		while(true) {
			int pid = CSystemTools::doWait(&status);

			if (pid < 0) {
				break;
			}

			cout << "ThrPid: " << pid << " = " << status << endl;
		}

		CSystemTools::doSleep(10);
	}

	return 0;
}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */


