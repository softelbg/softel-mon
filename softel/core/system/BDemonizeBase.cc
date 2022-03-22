/*
 * BDemonizeBase.cc
 *
 *  Created on: Apr 6, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BDemonizeBase.h"

using namespace std;
using namespace SF::CORE::SYSTEM;
using namespace SF::CORE::ERROR;

namespace SF {
namespace CORE {
namespace SYSTEM {

BDemonizeBase::BDemonizeBase() {
}

BDemonizeBase::~BDemonizeBase() {
	// TODO Auto-generated destructor stub
}

/**
 * Start the demonizer.
 */
void BDemonizeBase::start() {

	int pidMainProcess = 0;
	int pidChildProcess = 0;
	int pidWaitedChildProcess = 0;
	int childStatus = 0;

	CSystemTools::doInitialFork();

	try {
		pidMainProcess = CSystemTools::doGetPid();
		cout << "Main Process PID: " << pidMainProcess << endl;

		while (true) {
			pidChildProcess = CSystemTools::doFork();

			// Child Process should start running...
			if (pidChildProcess == 0) {

				this->runDemonizedServer();

				break;
			}

			// Main Process
			pidWaitedChildProcess = CSystemTools::doWait(&childStatus);

			cout << "Main Process: Child:" << pidWaitedChildProcess
					<< " Exited with status:" << childStatus << endl;

			CSystemTools::doSleep(1);
		}

	} catch (std::exception& e) {
		std::cerr << "exception: " << e.what() << "\n";
	} catch (CErrorSql& error) {
		//LOGHCS(LOG_LEVEL_ERROR, error.getMessage());
		std::cerr << "exception: " << error.getMessage() << endl;
	} catch (CErrorCommon& error) {
		//LOGHCS(LOG_LEVEL_ERROR, error.getMessage());
		std::cerr << "exception: " << error.getMessage() << endl;
	}

}

} /* namespace SYSTEM */
} /* namespace CORE */
} /* namespace SF */
