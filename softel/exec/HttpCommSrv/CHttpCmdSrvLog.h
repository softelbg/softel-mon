/*
 * CHttpCmdSrvLog.h
 *
 *  Created on: 05.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CHTTPCMDSRVLOG_H_
#define CHTTPCMDSRVLOG_H_

#include <softel/core/util/BLogFileBase.h>

namespace SF {
namespace OSS {
namespace EXEC {

#define LOGHCSD		(*(CHttpCmdSrvLog::getInstance()))
#define LOGHCS(a, b) CHttpCmdSrvLog::getInstance()->log(a, b)

class CHttpCmdSrvLog: public SF::CORE::UTIL::BLogFileBase {
public:
	virtual ~CHttpCmdSrvLog();

	static CHttpCmdSrvLog* getInstance();

private:
	CHttpCmdSrvLog();

	static CHttpCmdSrvLog* m_pInstance;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CHTTPCMDSRVLOG_H_ */
