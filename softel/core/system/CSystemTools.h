/*
 * CSystemTools.h
 *
 * Wrapper for various system functions.
 *
 *  Created on: 20.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSYSTEMTOOLS_H_
#define CSYSTEMTOOLS_H_

#include <stdio.h>
#include <time.h>
#include <sys/wait.h>

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include <unistd.h>

namespace SF {
namespace CORE {
namespace SYSTEM {

class CSystemTools {

public:

	CSystemTools();

	static int doFork();
	static void doInitialFork();
	static int doWait(int* pStatus);
	static void doExit(int status);
	static int doGetPid();
	static void doSystem(const std::string& cmd);

	static void doSleep(int period);
	static void doUSleep(int period);

	static long doGetCurrentTimeUx();
	static std::string getCTimeNow();

};

} /* namespace SYSTEM */
} /* namespace CORE */
} /* namespace SF */


#endif /* CSYSTEMTOOLS_H_ */
