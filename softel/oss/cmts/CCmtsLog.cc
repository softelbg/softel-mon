/*
 * CCmtsLog.cc
 *
 *  Created on: 04.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCmtsLog.h"

using namespace std;

namespace SF {
namespace OSS {
namespace CMTS {

#define LOG_FILE_PATH	"/var/log/CMTSMon.log"

CCmtsLog* CCmtsLog::m_pInstance = 0;

CCmtsLog::CCmtsLog() :
	BLogFileBase(LOG_FILE_PATH) {

}

CCmtsLog::~CCmtsLog() {
	// TODO Auto-generated destructor stub
}

CCmtsLog* CCmtsLog::getInstance() {

	if (m_pInstance == NULL) {
		m_pInstance = new CCmtsLog();
	}

	return m_pInstance;
}


} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */
