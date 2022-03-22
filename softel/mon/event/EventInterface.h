/*
 * EventInterface.h
 *
 *  Created on: 01.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef EVENTINTERFACE_H_
#define EVENTINTERFACE_H_

#include <string>
#include <softel/core/util/DateTime.h>

namespace SF {
namespace MON {
namespace EVENT {

enum EventLevel {
	ERROR,
	WARN,
	INFO,
	DEBUG
};

/*
 *
 */
class EventInterface {
public:
	EventInterface();
	virtual ~EventInterface();

	virtual std::string getName() const = 0;
	virtual EventLevel getLevel() const = 0;
	virtual std::string getType() const = 0;
	virtual const SF::CORE::UTIL::DateTime getDateTime() const = 0;

};

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
#endif /* EVENTINTERFACE_H_ */
