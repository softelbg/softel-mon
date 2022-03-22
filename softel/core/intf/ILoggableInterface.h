/*
 * ILoggableInterface.h
 *
 *	LOG Interface.
 *
 *  Created on: 03.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef ILOGGABLEINTERFACE_H_
#define ILOGGABLEINTERFACE_H_

#include <string>

namespace SF {
namespace CORE {
namespace INTF {

#define LOG_LEVEL_DEBUG		10
#define LOG_LEVEL_INFO		15
#define LOG_LEVEL_WARN		20
#define LOG_LEVEL_ERROR		30

class ILoggableInterface {
public:
	ILoggableInterface();
	virtual ~ILoggableInterface();

	virtual void log(const std::string& message)=0;
	virtual void log(int level, const std::string& message)=0;

};

} /* namespace INTF */
} /* namespace CORE */
} /* namespace SF */

#endif /* ILOGGABLEINTERFACE_H_ */
