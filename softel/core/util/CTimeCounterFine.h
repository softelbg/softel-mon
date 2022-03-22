/*
 * CTimeCounterFine.h
 *
 * Fine precision time counter.
 *
 *  Created on: 19.08.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTIMECOUNTERFINE_H_
#define CTIMECOUNTERFINE_H_

#include <stdio.h>
#include <time.h>

#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

#include "CTimeCounter.h"

namespace SF {
namespace CORE {
namespace UTIL {

class CTimeCounterFine: public CTimeCounter {
public:
	CTimeCounterFine();
	virtual ~CTimeCounterFine();

	virtual void reset();
	virtual void start();
	virtual void stop();

	double getTimeNow();
	double getTimeStart();
	double getTimeStop();

	double getTimeElapsed();

private:

	double m_time_start;
	double m_time_stop;
	double m_time_now;

};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

#endif /* CTIMECOUNTERFINE_H_ */
