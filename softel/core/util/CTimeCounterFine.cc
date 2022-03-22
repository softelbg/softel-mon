/*
 * CTimeCounterFine.cc
 *
 *  Created on: 19.08.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CTimeCounterFine.h"

namespace SF {
namespace CORE {
namespace UTIL {

CTimeCounterFine::CTimeCounterFine() {
	// TODO Auto-generated constructor stub

}

CTimeCounterFine::~CTimeCounterFine() {
	// TODO Auto-generated destructor stub
}

void CTimeCounterFine::reset() {

	this->getTimeNow();

	this->m_time_start = this->m_time_now;

	this->m_time_stop = 0;

}

void CTimeCounterFine::start() {
	this->reset();
}

void CTimeCounterFine::stop() {

	this->getTimeNow();

	this->m_time_stop = this->m_time_now;

}

/**
 * Calculate current time including nanoseconds.
 *
 */
double CTimeCounterFine::getTimeNow() {

	timespec time;

	clock_gettime(CLOCK_REALTIME, &time);

	this->m_time_now = time.tv_sec + (double)time.tv_nsec / 1000000000.0;

	return this->m_time_now;
}

double CTimeCounterFine::getTimeStart() {
	return this->m_time_start;
}

double CTimeCounterFine::getTimeStop() {
	return this->m_time_stop;
}

double CTimeCounterFine::getTimeElapsed() {
	return this->m_time_stop - this->m_time_start;
}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */



