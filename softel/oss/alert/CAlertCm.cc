/*
 * CAlertCm.cc
 *
 *  Created on: 24.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CAlertCm.h"

using namespace std;
using namespace SF::OSS::CMTS;
using namespace SF::DB;

namespace SF {
namespace OSS {
namespace ALERT {

CAlertCm::CAlertCm(CCmtsCm* pCm) {

	setCm(pCm);

	initCmParams();

}

CAlertCm::~CAlertCm() {

}

void CAlertCm::setCm(CCmtsCm* pCm) {
	m_pCm = pCm;
}

void CAlertCm::saveAlert() {

}

void CAlertCm::doNewAlert() {

	COssAlert::saveAlert();

	m_dbTools.sqlInsertAlertCm();

}

/**
 * Initialize alert params.
 */
void CAlertCm::initAlert() {
	setAlertType(TYPE_ALERT_CMTS_CM);
	setAlertSeverity(SEVERITY_ALERT_MIDDLE);
}

/**
 * Initialize Alert from a Data Row.
 *
 */
void CAlertCm::initAlert(CDataRow& row) {

	COssAlert::initAlert(row);

	this->initAlert();
}

/**
 * Copy needed params from CM to Alert Param Set
 */
void CAlertCm::initCmParams() {

	//m_pCm->dumpCmInfo();

	m_param_set.setParam(KEY_SNMP_OID_CM_CMTS_ID, m_pCm->getCmtsId());
	m_param_set.setParam(KEY_SNMP_OID_CM_MAC_DEC, m_pCm->getParam(KEY_SNMP_OID_CM_MAC_DEC));

	m_param_set.setParam(PARAM_ALERT_MESSAGE, m_pCm->getParam(KEY_PARAM_EVENT_MESSAGE));

}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

