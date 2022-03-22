/*
 * CCmtsAlertLog.cc
 *
 *  Created on: 05.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCmtsAlertLog.h"

using namespace std;

namespace SF {
namespace OSS {
namespace CMTS {

#define LOG_FILE_PATH	"/var/log/CmtsAlert.log"

CCmtsAlertLog* CCmtsAlertLog::m_pInstance = 0;

CCmtsAlertLog::CCmtsAlertLog() :
	BLogFileBase(LOG_FILE_PATH) {

}

CCmtsAlertLog::~CCmtsAlertLog() {
	// TODO Auto-generated destructor stub
}

CCmtsAlertLog* CCmtsAlertLog::getInstance() {

	if (m_pInstance == NULL) {
		m_pInstance = new CCmtsAlertLog();
	}

	return m_pInstance;
}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

