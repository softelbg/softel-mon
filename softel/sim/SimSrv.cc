/*
 * SimSrv.cc
 *
 *  Created on: 25.08.2013
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include <vector>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CSimSrvLog.h"
#include "SimMain.h"

#include <softel/core/error/CErrorCommon.h>
#include <softel/core/error/CErrorSql.h>
#include <softel/core/system/CSystemTools.h>

using namespace std;
using namespace SF::CORE::SYSTEM;
using namespace SF::CORE::ERROR;
using namespace SF::SIM;

int main(int argc, char *argv[]) {

	int pidMainProcess = 0;
	int pidChildProcess = 0;
	int pidWaitedChildProcess = 0;
	int childStatus = 0;

	CSystemTools::doInitialFork();

	try {

		pidMainProcess = CSystemTools::doGetPid();

		LOGSIMD << "Main Process PID: " << pidMainProcess << endl;

		while (true) {

			pidChildProcess = CSystemTools::doFork();

			if (pidChildProcess == 0) {

				SimMain simSrv;
				simSrv.start();

				break;
			}

			// Main Process
			pidWaitedChildProcess = CSystemTools::doWait(&childStatus);

			LOGSIMD << "Main Process: Child:" << pidWaitedChildProcess << " Exited with status:" << childStatus << endl;

			CSystemTools::doSleep(1);
		}

	} catch (CErrorSql& error) {
		LOGSIMS(LOG_LEVEL_ERROR, error.getMessage());
	} catch (CErrorCommon& error) {
		LOGSIMS(LOG_LEVEL_ERROR, error.getMessage());
	}

	return 0;
}

