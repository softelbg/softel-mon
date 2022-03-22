/*
 * EventBase.h
 *
 *  Created on: 01.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef EVENTBASE_H_
#define EVENTBASE_H_

#include <softel/core/CObject.h>
#include "EventInterface.h"

namespace SF {
namespace MON {
namespace EVENT {

/*
 *
 */
class EventBase: public virtual SF::MON::EVENT::EventInterface,
					public SF::CORE::CObject {
public:
	EventBase();
	EventBase(	const std::string& name,
				const EventLevel& level,
				const std::string& type);
	virtual ~EventBase();

	virtual std::string getName() const {
		return this->m_name;
	}
	virtual EventLevel getLevel() const {
		return this->m_level;
	}
	std::string getType() const {
		return this->m_type;
	}

	const SF::CORE::UTIL::DateTime getDateTime() const {
		return this->m_date_created;
	}

	std::string toString() const;

protected:
	std::string		m_name;
	EventLevel		m_level;
	std::string		m_type;

	SF::CORE::UTIL::DateTime m_date_created;


};

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
#endif /* EVENTBASE_H_ */
