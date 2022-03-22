/*
 * CLaunchSnmpFromCm.cc
 *
 *  Created on: 25.01.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */


#include "CLaunchSnmpFromCm.h"


using namespace std;
using namespace SF::DB;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::NET;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace OSS {
namespace CMTS {

CLaunchSnmpFromCm::CLaunchSnmpFromCm(CCmtsCm* cm, vector<CSnmpOid>& snmp_oid_cm) :
	m_snmp_oid_cm(snmp_oid_cm) {

	m_pCm = cm;

}

CLaunchSnmpFromCm::~CLaunchSnmpFromCm() {

}

int CLaunchSnmpFromCm::run() {

	cout << "Thr:" << getThrId() << endl;
	m_pCm->dumpCmInfo();

	m_pCm->doCmSnmpGet(m_pCm->getDbConn(), m_pCm->getCmIpTools(), m_snmp_oid_cm);

//	m_pCm->closeCmSnmpSession();

	cout << "Thr:" << getThrId() << " Ended" << endl;
	return 0;
}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */
