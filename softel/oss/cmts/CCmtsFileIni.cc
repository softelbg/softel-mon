/*
 * CCmtsFileIni.cc
 *
 *  Created on: 29.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCmtsFileIni.h"

using namespace std;

namespace SF {
namespace OSS {
namespace CMTS {

#define PATH_CMTS_MON_INI	"/etc/CMTSMon.ini"

CCmtsFileIni* CCmtsFileIni::m_pInstance = NULL;

CCmtsFileIni::CCmtsFileIni() :
	BFileIniBase(PATH_CMTS_MON_INI) {

}

CCmtsFileIni::~CCmtsFileIni() {
	// TODO Auto-generated destructor stub
}

CCmtsFileIni* CCmtsFileIni::getInstance() {

	if (m_pInstance == NULL) {
		m_pInstance = new CCmtsFileIni();
		//m_pInstance->dumpInfo();
	}

	return m_pInstance;
}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */
