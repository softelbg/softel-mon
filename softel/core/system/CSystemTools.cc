/*
 * CSystemTools.cc
 *
 *  Created on: 20.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSystemTools.h"

using namespace std;

namespace SF {
namespace CORE {
namespace SYSTEM {

CSystemTools::CSystemTools() {

}

int CSystemTools::doFork() {
	return fork();
}

int CSystemTools::doWait(int* pStatus) {
	return wait(pStatus);
}

void CSystemTools::doExit(int status) {
	exit(status);
}

int CSystemTools::doGetPid() {
	return getpid();
}

void CSystemTools::doSystem(const string& cmd) {
	system(cmd.c_str());
}

void CSystemTools::doSleep(int period) {
	sleep(period);
}

void CSystemTools::doUSleep(int period) {
	usleep(period);
}

long CSystemTools::doGetCurrentTimeUx() {
	return time(0);
}

void CSystemTools::doInitialFork() {
	if (CSystemTools::doFork() != 0) {
		CSystemTools::doExit(0);
	}
}

string CSystemTools::getCTimeNow() {

	time_t time_now;

	time(&time_now);

	return string(ctime(&time_now));
}

} /* namespace SYSTEM */
} /* namespace CORE */
} /* namespace SF */
