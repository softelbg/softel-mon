/*
 * CTimeCounter.h
 *
 * Time Counter
 *
 * Could measure some time period or display current time.
 *
 *  Created on: 07.03.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTIMECOUNTER_H_
#define CTIMECOUNTER_H_

#include <stdio.h>
#include <time.h>

#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

namespace SF {
namespace CORE {
namespace UTIL {

class CTimeCounter {

public:

	CTimeCounter();
	virtual ~CTimeCounter();

	virtual void reset();
	virtual void start();
	virtual void stop();

	std::string getCTimeStart();
	std::string getCTimeNow();

	time_t getStoppedElapsed();
	time_t getElapsed();
	std::string getElapsedFormated();

	time_t getTimeStart();
	time_t getTimeStop();

private:

	time_t m_time;
	time_t m_time_now;
	time_t m_time_stop;

};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

#endif /* CTIMECOUNTER_H_ */
