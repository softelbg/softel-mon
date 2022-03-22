/*
 * CTimeCounter.cc
 *
 *  Created on: 07.03.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CTimeCounter.h"

using namespace std;

namespace SF {
namespace CORE {
namespace UTIL {

CTimeCounter::CTimeCounter() {

	reset();

}

CTimeCounter::~CTimeCounter() {

}

void CTimeCounter::reset() {

	time(&m_time);

	this->m_time_now = this->m_time;
	this->m_time_stop = 0;
}

void CTimeCounter::start() {
	reset();
}

void CTimeCounter::stop() {

	time(&m_time_stop);

}

string CTimeCounter::getCTimeStart() {
	return string(ctime(&m_time));
}

string CTimeCounter::getCTimeNow() {

	time(&m_time_now);

	return string(ctime(&m_time_now));
}

time_t CTimeCounter::getStoppedElapsed() {
	return this->m_time_stop - this->m_time;
}

time_t CTimeCounter::getElapsed() {

	time(&m_time_now);

	return m_time_now - m_time;
}

string CTimeCounter::getElapsedFormated() {

	stringstream ss;

	ss << "Elapsed " << getElapsed() << " s ";

	return ss.str();
}

time_t CTimeCounter::getTimeStart() {
	return this->m_time;
}

time_t CTimeCounter::getTimeStop() {
	return this->m_time_stop;
}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */
