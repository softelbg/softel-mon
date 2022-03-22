/*
 * CHttpCmdSrvLog.cc
 *
 *  Created on: 05.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CHttpCmdSrvLog.h"

namespace SF {
namespace OSS {
namespace EXEC {

#define LOG_FILE_PATH	"/var/log/HttpCmdSrv.log"

CHttpCmdSrvLog* CHttpCmdSrvLog::m_pInstance = 0;

CHttpCmdSrvLog::CHttpCmdSrvLog() :
			BLogFileBase(LOG_FILE_PATH) {

}

CHttpCmdSrvLog::~CHttpCmdSrvLog() {
	// TODO Auto-generated destructor stub
}

CHttpCmdSrvLog* CHttpCmdSrvLog::getInstance() {

	if (m_pInstance == NULL) {
		m_pInstance = new CHttpCmdSrvLog();
	}

	return m_pInstance;
}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

