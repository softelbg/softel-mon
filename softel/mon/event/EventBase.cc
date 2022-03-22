/*
 * EventBase.cc
 *
 *  Created on: 01.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "EventBase.h"
#include <typeinfo>
#include <sstream>
#include <softel/core/system/CSystemTools.h>

namespace SF {
namespace MON {
namespace EVENT {

EventBase::EventBase() :
		m_level(EventLevel::INFO), m_name("") {

	m_name = typeid(*this).name();
	this->m_date_created.setNow();
}

EventBase::EventBase(const std::string& name, const EventLevel& level,
		const std::string& type) :
		m_name(name), m_level(level), m_type(type) {
	this->m_date_created.setNow();
}

EventBase::~EventBase() {
	// TODO Auto-generated destructor stub
}

std::string EventBase::toString() const {
	std::stringstream ss;

	ss << "name:" << this->getName();
	ss << " level: " << this->getLevel();
	ss << " type: " << this->getType();
	ss << " time: " << this->getDateTime().getTimeStamp();
	ss << " elapsed: " << this->getDateTime().getElapsed();

	return ss.str();
}

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
