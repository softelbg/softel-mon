/*
 * CSimSrvLog.h
 *
 *  Created on: 05.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSIMSRVLOG_H_
#define CSIMSRVLOG_H_

#include <softel/core/util/BLogFileBase.h>

namespace SF {
namespace SIM {

#define LOGSIMD		(*(CSimSrvLog::getInstance()))
#define LOGSIMS(a, b) CSimSrvLog::getInstance()->log(a, b)

class CSimSrvLog: public SF::CORE::UTIL::BLogFileBase {
public:
	virtual ~CSimSrvLog();

	static CSimSrvLog* getInstance();

private:
	CSimSrvLog();

	static CSimSrvLog* m_pInstance;

};

} /* namespace SIM */
} /* namespace SF */

#endif /* CSIMSRVLOG_H_ */
