/*
 * CScheduler.cc
 *
 *  Created on: 21.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CScheduler.h"

#include "CTimeCounter.h"

#include <softel/core/system/CSystemTools.h>
#include <softel/core/system/CFork.h>
#include <softel/core/error/CErrorCommon.h>
#include <softel/core/error/CErrorSql.h>

using namespace std;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace CORE {
namespace UTIL {

CScheduler::CScheduler() {

	setPeriodSleep(600);

}

CScheduler::~CScheduler() {

}

/**
 * Start a Monitoring Cycle.
 * Fork and start monitoring
 */
void CScheduler::startCycle() {

	CFork f;

	f.doFork();

	if (f.isChild()) {

		run();

	}

}

void CScheduler::setPeriodSleep(int period) {
	m_periodSleep = period;
}

int CScheduler::getMainProcessPid() {
	return m_pidMainProcess;
}

/**
 * Start Scheduler
 *
 */
void CScheduler::start() {

	int childStatus = 0;

	CTimeCounter tcMain;
	CTimeCounter tcChild;

	m_pidMainProcess = CSystemTools::doGetPid();

	cout << "Main Process PID: " << m_pidMainProcess << endl;

	while (true) {

		cout << "pid:" << CSystemTools::doGetPid() << " Elapsed "
				<< tcMain.getElapsed() << " s  " << tcMain.getCTimeNow()
				<< endl;

		tcChild.reset();

		try {

			startCycle();

		} catch (CErrorSql& error) {
			cout << "CErrorSql: " << error.getMessage() << endl;
		} catch (CErrorCommon& error) {
			cout << "Error: " << error.getMessage() << endl;
		};

		cout << "pid:" << CSystemTools::doGetPid();
		cout << " Elapsed " << tcChild.getElapsed() << " s  "
				<< tcChild.getCTimeNow() << endl;

		if (m_pidMainProcess == CSystemTools::doGetPid()) {

			pid_t childPid;

			do {

				childPid = CSystemTools::doWait(&childStatus);

				cout << "Child pid:" << childPid << " Ended with status:"
						<< childStatus;
				cout << " Elapsed " << tcChild.getElapsed() << " s  "
						<< tcChild.getCTimeNow() << endl;

			} while (childPid > 0);

			childPid = CSystemTools::doWait(&childStatus);
			cout << "Final Child pid:" << childPid << " Ended with status:"
					<< childStatus;
			cout << " Elapsed " << tcMain.getElapsed() << " s  "
					<< tcMain.getCTimeNow() << endl;

			// Wait if there is remaining time for sleeping
			time_t elapsed = tcMain.getElapsed();
			int remainingSleep = m_periodSleep - elapsed;

			cout << "remainingSleep: " << remainingSleep << " m_periodSleep: "
					<< " elapsed: " << elapsed << endl;

			if (remainingSleep > 0) {
				cout << "Main Process Sleeping for " << remainingSleep
						<< " s..." << endl;

				CSystemTools::doSleep(remainingSleep);
			}

			tcMain.reset();

		} else {
			break;
		}

	}

}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

