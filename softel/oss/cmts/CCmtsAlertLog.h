/*
 * CCmtsAlertLog.h
 *
 *  Created on: 05.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCMTSALERTLOG_H_
#define CCMTSALERTLOG_H_

#include <softel/core/util/BLogFileBase.h>

namespace SF {
namespace OSS {
namespace CMTS {

#define LOGAD		(*(CCmtsAlertLog::getInstance()))
#define LOGA(a, b) CCmtsAlertLog::getInstance()->log(a, b)

class CCmtsAlertLog: public SF::CORE::UTIL::BLogFileBase {
public:
	virtual ~CCmtsAlertLog();

	static CCmtsAlertLog* getInstance();

private:
	CCmtsAlertLog();

	static CCmtsAlertLog* m_pInstance;

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CCMTSALERTLOG_H_ */
