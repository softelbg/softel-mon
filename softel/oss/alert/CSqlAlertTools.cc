/*
 * CSqlAlertTools.cc
 *
 *  Created on: 01.08.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSqlAlertTools.h"

using namespace std;
using namespace SF::DB;

namespace SF {
namespace OSS {
namespace ALERT {

CSqlAlertTools::CSqlAlertTools() {

}

CSqlAlertTools::~CSqlAlertTools() {

}

void CSqlAlertTools::setAlert(CParamSet* pParamSet) {
	m_pParamSet = pParamSet;
}

/**
 * Insert into DB the Alert.
 * TODO: Consider counter check for existence and only update if necessary.
 */
void CSqlAlertTools::sqlInsertAlertBase() {

	int result = 0;

	stringstream sqlAlert;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;

	// TODO: Still only testing phase... Consider make auto param list...
	sqlAlert << "INSERT INTO " << TABLE_OSS_ALERT_BASE;
	sqlAlert << " (atype, astatus, acounter, adate_init, aseverity, amessage) VALUES ";
	sqlAlert << " (" << m_pParamSet->getParam(PARAM_ALERT_TYPE) << ", ";
	sqlAlert << m_pParamSet->getParam(PARAM_ALERT_STATUS) << ", ";
	sqlAlert << m_pParamSet->getParam(PARAM_ALERT_COUNTER) << ", ";
	sqlAlert << m_pParamSet->getParam(PARAM_ALERT_DATE_INIT) << ", ";
	sqlAlert << m_pParamSet->getParam(PARAM_ALERT_SEVERITY) << ", ";
	sqlAlert << "\'" << m_pParamSet->getParam(PARAM_ALERT_MESSAGE) << "\') ";

	// TODO: decide which is better - already created sql conn or created on every sql exec...
	result = sqlInsert(sqlAlert);

	m_pParamSet->setParam(PARAM_ALERT_ID, result);

}

/**
 * Inset into DB a CM Alert.
 */
void CSqlAlertTools::sqlInsertAlertCm() {

	int result = 0;

	stringstream sqlAlert;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;

	sqlAlert << "INSERT INTO " << TABLE_OSS_ALERT_CM;
	sqlAlert << " (aid, cm_mac, cmts_id) VALUES ";
	sqlAlert << " (" <<  m_pParamSet->getParam(PARAM_ALERT_ID) << ", ";
	sqlAlert << m_pParamSet->getParam(KEY_SNMP_OID_CM_MAC_DEC) << ", ";
	sqlAlert << m_pParamSet->getParam(KEY_SNMP_OID_CM_CMTS_ID) << ") ";

	// TODO: decide which is better - already created sql conn or created on every sql exec...
	result = sqlInsert(sqlAlert);

}

/**
 * Select a dedicated CM Active Alert if exists
 */
void CSqlAlertTools::sqlSelectAlertCmDedicated(CDataSet& ds) {

	stringstream sqlAlert;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;

	sqlAlert << "SELECT * FROM " << TABLE_OSS_ALERT_BASE << ", " << TABLE_OSS_ALERT_CM;
	sqlAlert << " WHERE "TABLE_OSS_ALERT_BASE".aid = "TABLE_OSS_ALERT_CM".aid ";
	sqlAlert << " AND cmts_id = " << m_pParamSet->getParam(KEY_SNMP_OID_CM_CMTS_ID);
	sqlAlert << " AND cm_mac = " << m_pParamSet->getParam(KEY_SNMP_OID_CM_MAC_DEC);
	sqlAlert << " AND atype = " << TYPE_ALERT_CM_DEDICATED;
	sqlAlert << " AND astatus <= 1";

	sqlSelect(sqlAlert, ds);

}

/**
 * Increment the counter of base alert.
 * Update some of the params.
 */
void CSqlAlertTools::sqlIncAlertBase() {

	stringstream sqlAlert;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;

	sqlAlert << "UPDATE " << TABLE_OSS_ALERT_BASE;
	sqlAlert << " SET acounter = acounter + 1, ";
	sqlAlert << " aseverity = " << m_pParamSet->getParam(PARAM_ALERT_SEVERITY) << ", ";
	sqlAlert << " amessage = \'" << m_pParamSet->getParam(PARAM_ALERT_MESSAGE) << "\'";
	sqlAlert << " WHERE aid = " << m_pParamSet->getParam(PARAM_ALERT_ID);

	sqlExecute(sqlAlert);

}

/**
 * Update Base Alert according to alert params.
 * TODO: Consider make it auto update set list...
 */
void CSqlAlertTools::sqlUpdateAlertBase() {

	stringstream sqlAlert;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;

	sqlAlert << "UPDATE " << TABLE_OSS_ALERT_BASE;
	sqlAlert << " SET acounter = " << m_pParamSet->getParam(PARAM_ALERT_COUNTER) << ", ";
	sqlAlert << " astatus = " << m_pParamSet->getParam(PARAM_ALERT_STATUS) << ", ";
	sqlAlert << " acounter = " << m_pParamSet->getParam(PARAM_ALERT_COUNTER) << ", ";
	sqlAlert << " adate = " << m_pParamSet->getParam(PARAM_ALERT_DATE) << ", ";
	sqlAlert << " aseverity = " << m_pParamSet->getParam(PARAM_ALERT_SEVERITY) << ", ";
	sqlAlert << " amessage = \'" << m_pParamSet->getParam(PARAM_ALERT_MESSAGE) << "\'";
	sqlAlert << " WHERE aid = " << m_pParamSet->getParam(PARAM_ALERT_ID);

	sqlExecute(sqlAlert);
}

void CSqlAlertTools::sqlInsertAlertIf() {

	int result = 0;

	stringstream sqlAlert;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;

	sqlAlert << "INSERT INTO " << TABLE_OSS_ALERT_IF;
	sqlAlert << " (aid, cmts_id, if_id) VALUES ";
	sqlAlert << " (" << m_pParamSet->getParam(PARAM_ALERT_ID) << ", ";
	sqlAlert << m_pParamSet->getParam(KEY_COL_CMTS_ID) << ", ";
	sqlAlert << m_pParamSet->getParam(KEY_COL_IF_ID) << ") ";

	result = sqlInsert(sqlAlert);

}

void CSqlAlertTools::sqlSelectAlertCmtsIfCounter(CDataSet& ds) {

	stringstream sqlAlert;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;

	sqlAlert << "SELECT * FROM " << TABLE_OSS_ALERT_BASE << ", " << TABLE_OSS_ALERT_IF;
	sqlAlert << " WHERE "TABLE_OSS_ALERT_BASE".aid = "TABLE_OSS_ALERT_IF".aid ";
	sqlAlert << " AND cmts_id = " << m_pParamSet->getParam(KEY_COL_CMTS_ID);
	sqlAlert << " AND if_id = " << m_pParamSet->getParam(KEY_COL_IF_ID);
	sqlAlert << " AND atype = " << TYPE_ALERT_CMTS_IF_COUNTER;
	sqlAlert << " AND astatus <= 1";

	sqlSelect(sqlAlert, ds);
}

/**
 * Select CmtsIfCmCounter List of active Alerts.
 *
 */
void CSqlAlertTools::sqlSelectAlertCmtsIfCounterList(CDataSet& ds) {

	stringstream sqlAlert;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;

	sqlAlert << "SELECT * FROM " << TABLE_OSS_ALERT_BASE << ", " << TABLE_OSS_ALERT_IF;
	sqlAlert << " WHERE "TABLE_OSS_ALERT_BASE".aid = "TABLE_OSS_ALERT_IF".aid ";
	sqlAlert << " AND atype = " << TYPE_ALERT_CMTS_IF_COUNTER;
	sqlAlert << " AND astatus <= 1";

	sqlSelect(sqlAlert, ds);
}

void CSqlAlertTools::sqlSelectCmIfDocs3List(CDataSet& ds) {

	stringstream sqlQuery;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;

	sqlQuery << "SELECT * FROM " << TABLE_OSS_CM_LIST_DOCS3;

	sqlSelect(sqlQuery, ds);

}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */


