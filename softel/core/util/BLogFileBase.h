/*
 * BLogFileBase.h
 *
 *	Base File Logger. Could be subclassed with concrete Log Files.
 *
 *  Created on: 03.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BLOGFILEBASE_H_
#define BLOGFILEBASE_H_

#include "BFileBase.h"
#include <softel/core/intf/ILoggableInterface.h>

namespace SF {
namespace CORE {
namespace UTIL {

class BLogFileBase: public BFileBase, public SF::CORE::INTF::ILoggableInterface {
public:
	BLogFileBase(const std::string& logFileName);
	virtual ~BLogFileBase();

	virtual void log(const std::string& message);
	virtual void log(int level, const std::string& message);

	static void log(const std::string& logFileName, const std::string& message);
	static void log(const std::string& logFileName, int level, const std::string& message);

protected:

	virtual std::string doLogLine(int level, const std::string& message);

};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

#endif /* BLOGFILEBASE_H_ */
