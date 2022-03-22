/*
 * CSimSrvLog.cc
 *
 *  Created on: 05.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSimSrvLog.h"

namespace SF {
namespace SIM {

#define LOG_FILE_PATH	"/var/log/sim.log"

CSimSrvLog* CSimSrvLog::m_pInstance = 0;

CSimSrvLog::CSimSrvLog() :
			BLogFileBase(LOG_FILE_PATH) {

}

CSimSrvLog::~CSimSrvLog() {
	// TODO Auto-generated destructor stub
}

CSimSrvLog* CSimSrvLog::getInstance() {

	if (m_pInstance == NULL) {
		m_pInstance = new CSimSrvLog();
	}

	return m_pInstance;
}

} /* namespace SIM */
} /* namespace SF */

