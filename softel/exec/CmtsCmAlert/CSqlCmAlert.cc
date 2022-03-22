/*
 * CSqlCmAlert.cc
 *
 *  Created on: 22.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSqlCmAlert.h"

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::CMTS;

namespace SF {
namespace OSS {
namespace EXEC {

CSqlCmAlert* CSqlCmAlert::m_pInstance = 0;

CSqlCmAlert::CSqlCmAlert() {

}

CSqlCmAlert* CSqlCmAlert::CSqlCmAlert::getInstance() {

	if (m_pInstance == NULL) {
		m_pInstance = new CSqlCmAlert();
	}

	return m_pInstance;
}

bool CSqlCmAlert::sqlGetCmList(CDataSet& cmList) {

	CMySqlDB* pDb = CMySqlConn::getInstance()->getDbFactory()->newMySqlConn();

	bool result = sqlGetCmList(pDb, cmList);

	delete pDb;

	return result;
}
/**
 * Do a Select for the CM Alert List
 */
bool CSqlCmAlert::sqlGetCmList(CMySqlDB* pDb, CDataSet& cmList) {

	stringstream sqlSelect;

	/*
	sqlSelect << "SELECT " << TABLE_OSS_ALERT_CM_LIST".cmts_id, ";
	sqlSelect << TABLE_OSS_ALERT_CM_LIST".cm_mac, ";
	sqlSelect << "cm_ip, cm_sid, cmts_rcommunity, cm_rcommunity, cmts_ip, cm_status ";
*/

	/*
	sqlSelect << "SELECT * ";
	sqlSelect << " FROM " << TABLE_OSS_ALERT_CM_LIST << ", " << TABLE_OSS_CM_LIST << ", " << TABLE_OSS_CMTS_LIST;
	sqlSelect << " WHERE " << TABLE_OSS_ALERT_CM_LIST".cmts_id = " << TABLE_OSS_CM_LIST".cmts_id ";
	sqlSelect << " AND " << TABLE_OSS_ALERT_CM_LIST".cmts_id = " << TABLE_OSS_CMTS_LIST".cmts_id ";
	sqlSelect << " AND " << TABLE_OSS_ALERT_CM_LIST".cm_mac = " << TABLE_OSS_CM_LIST".cm_mac ";
//	sqlSelect << " AND " << TABLE_OSS_CM_LIST".cm_status = " << STATUS_CM_REGISTRATION_COMPLETE;
	sqlSelect << " AND " << TABLE_OSS_CMTS_LIST".active = 1";
*/

	sqlSelect << "SELECT oss_alert_cm_list.cmts_id, oss_alert_cm_list.cm_mac, ";
	sqlSelect << " oss_cm_list.cm_ip, cmts_ip, cmts_rcommunity, cm_rcommunity ";
	sqlSelect << " FROM oss_alert_cm_list ";
	sqlSelect << " LEFT JOIN oss_cm_list ON oss_cm_list.cmts_id = oss_alert_cm_list.cmts_id ";
	sqlSelect << "   AND oss_cm_list.cm_mac = oss_alert_cm_list.cm_mac ";
	sqlSelect << " JOIN oss_cmts_list ON oss_cmts_list.cmts_id = oss_alert_cm_list.cmts_id ";
	sqlSelect << " WHERE oss_cmts_list.active = 1";

	cout << __FUNCTION__ << ": SQL: " << sqlSelect.str() << endl;

	pDb->doSelect(sqlSelect.str(), cmList);

	return true;
}

bool CSqlCmAlert::sqlGetCmAlertList(CDataSet& dsCmAlert) {

	CMySqlDB* pDb = CMySqlConn::getInstance()->getDbFactory()->newMySqlConn();

	bool result = sqlGetCmAlertList(pDb, dsCmAlert);

	delete pDb;

	return result;
}
/**
 * Get CM Active Alert List - below Closed status.
 * Create CM MAC Index.
 *
 */
bool CSqlCmAlert::sqlGetCmAlertList(CMySqlDB* pDb, CDataSet& dsCmAlert) {

	stringstream sqlSelect;

	sqlSelect << "SELECT * ";
	sqlSelect << " FROM " << TABLE_OSS_ALERT_BASE << ", " << TABLE_OSS_ALERT_CM;
	sqlSelect << " WHERE "TABLE_OSS_ALERT_BASE".aid = "TABLE_OSS_ALERT_CM".aid";
	sqlSelect << " AND "TABLE_OSS_ALERT_BASE".astatus < 3";

	pDb->doSelect(sqlSelect.str(), dsCmAlert);

	dsCmAlert.buildColumnIndex(KEY_COL_CM_MAC);

	return true;
}

bool CSqlCmAlert::doCmtsCmOidList(const string& cmtsId, vector<CSnmpOid>& listOidCm) {

	CMySqlDB* pDb = CMySqlConn::getInstance()->getDbFactory()->newMySqlConn();

	bool result = doCmtsCmOidList(pDb, cmtsId, listOidCm);

	delete pDb;

	return result;
}
/**
 * Do CM Alert Snmp Oid List.
 * Now is used current full CM Oid List.
 *
 * TODO: Consider leaving only dwn snr for example...
 *
 */
bool CSqlCmAlert::doCmtsCmOidList(CMySqlDB* pDb, const string& cmtsId, vector<CSnmpOid>& listOidCm) {

	CCmtsToolsSql sqlCmtsTools(cmtsId);

	return sqlCmtsTools.selectCmOidList(pDb, listOidCm);
}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */
