/*
 * CScheduler.h
 *
 *  Created on: 21.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSCHEDULER_H_
#define CSCHEDULER_H_

namespace SF {
namespace CORE {
namespace UTIL {

class CScheduler {

public:
	CScheduler();
	virtual ~CScheduler();

	void setPeriodSleep(int period);
	int getMainProcessPid();

	virtual void run() = 0; // Should be implemented in derived classes - should run the scheduled task

	void start();

private:

	void startCycle();

	int m_periodSleep;
	int m_pidMainProcess;

};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */


#endif /* CSCHEDULER_H_ */
