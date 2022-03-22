/*
 * CThresholdResult.cc
 *
 *  Created on: 04.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CThresholdResult.h"

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::CMTS;

namespace SF {
namespace OSS {
namespace ALERT {

CThresholdResult::CThresholdResult(CThreshold* pThreshold, CSnmpOid* pSnmpOid) :
	m_pThreshold(pThreshold), m_pSnmpOid(pSnmpOid) {


}

CThresholdResult::CThresholdResult()  :
	m_pThreshold(0), m_pSnmpOid(0) {
}

CThresholdResult::~CThresholdResult() {
	// TODO Auto-generated destructor stub
}

void CThresholdResult::setThreshold(CThreshold* pThreshold) {
	this->m_pThreshold = pThreshold;
}

void CThresholdResult::setSnmpOid(CSnmpOid* pSnmpOid) {
	this->m_pSnmpOid = pSnmpOid;
}

void CThresholdResult::setCmRaw(CDataRow& row) {
	row.copyRow(this->m_cm_raw);
}

CThreshold* CThresholdResult::getThreshold() {
	return this->m_pThreshold;
}

CSnmpOid* CThresholdResult::getSnmpOid() {
	return this->m_pSnmpOid;
}

const CDataRow& CThresholdResult::getRow() {
	return this->m_cm_raw;
}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */




