/*
 * CCmtsLog.h
 *
 *  Created on: 04.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCMTSLOG_H_
#define CCMTSLOG_H_

#include <softel/core/util/BLogFileBase.h>

namespace SF {
namespace OSS {
namespace CMTS {

#define LOGD		(*(CCmtsLog::getInstance()))
#define LOG(a, b) CCmtsLog::getInstance()->log(a, b)

class CCmtsLog: public SF::CORE::UTIL::BLogFileBase {
public:
	virtual ~CCmtsLog();

	static CCmtsLog* getInstance();

private:
	CCmtsLog();

	static CCmtsLog* m_pInstance;

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CCMTSLOG_H_ */
