/*
 * DateTime.cc
 *
 *  Created on: Sep 17, 2013
 *      Author: stan
 */

#include "DateTime.h"
#include <softel/core/system/CSystemTools.h>

namespace SF {
namespace CORE {
namespace UTIL {

DateTime::DateTime() {
	setNow();
}

DateTime::DateTime(long timestamp) :
		m_timestamp(timestamp) {
}

DateTime::~DateTime() {
	// TODO Auto-generated destructor stub
}

long DateTime::getTimeStamp() const {
	return this->m_timestamp;
}

void DateTime::setTimeStamp(long timestamp) {
	this->m_timestamp = timestamp;
}

long DateTime::getElapsed() const {
	return SF::CORE::SYSTEM::CSystemTools::doGetCurrentTimeUx() - this->m_timestamp;
}

void DateTime::setNow() {
	setTimeStamp(SF::CORE::SYSTEM::CSystemTools::doGetCurrentTimeUx());
}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */
