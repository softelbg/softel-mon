/**
 * CMTS Cable Modem Representation
 *
 *	@author sgeorgiev
 *
 *		www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCmtsCm.h"

#include <softel/core/error/CErrorSql.h>
#include <softel/core/error/CErrorCommon.h>
#include <softel/oss/snmp/CSnmpCmIp.h>

using namespace std;
using namespace SF::OSS::SNMP;
using namespace SF::DB;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::NET;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::SYSTEM;


namespace SF {
namespace OSS {
namespace CMTS {

CCmtsCm::CCmtsCm()
	: m_Ip(NULL), m_pCmIp(NULL) {

}

CCmtsCm::CCmtsCm(const string& 	cm_mac_d,
				const int 		cmts_id) {

	setCmtsId(cmts_id);
	setCmMacDecimal(cm_mac_d);
}

CCmtsCm::CCmtsCm(	const string& 	cm_mac_d,
					const int 		cmts_id,
					const string& 	cmts_rcommunity,
					const string& 	cm_rcommunity) :
			m_Ip(NULL),
			m_pCmIp(NULL) {

	setRCommunity(cmts_rcommunity, cm_rcommunity);

	setCmtsId(cmts_id);
	setCmMacDecimal(cm_mac_d);
}

CCmtsCm::CCmtsCm(	const string& 	cm_mac_d,
					const string&	cm_sid,
					const int 		cmts_id,
					const string& 	cmts_rcommunity,
					const string& 	cm_rcommunity) :
			m_Ip(NULL),
			m_pCmIp(NULL) {

	setRCommunity(cmts_rcommunity, cm_rcommunity);
	setCmtsId(cmts_id);
	setCmSid(cm_sid);
	setCmMacDecimal(cm_mac_d);
}

CCmtsCm::CCmtsCm(	const char* cm_mac, const int cmts_id,
					const string& cmts_rcommunity, const string& cm_rcommunity) :
			m_Ip(NULL),
			m_pCmIp(NULL) {

	setRCommunity(cmts_rcommunity, cm_rcommunity);
	setCmtsId(cmts_id);
	// Convert MAC....
	setCmMac(cm_mac);

}

CCmtsCm::~CCmtsCm() {

	//delete m_pCmIp;

}

void CCmtsCm::addCpe(const CCpe& cpe) {
	m_cpe_list.addCpe(cpe);
}

/**
 * Make list of columns for CM List.
 * Should response to: where is_fn = 0 and CHAR_LENGTH(col_key) > 0 and from_cm < 2 and prio < 100
 *
 */
void CCmtsCm::doCmListAuxColumns(CDataRow& row) {

	unsigned int i;

	vector<CSnmpOid>& rSnmpOidList = *m_pSnmpOidList;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << " oidSize:" << rSnmpOidList.size() << endl;

	for(i = 0; i < rSnmpOidList.size(); i++) {

		// Filter oids according to cm list requirements
		if (	rSnmpOidList[i].isFunction() == false &&
			rSnmpOidList[i].hasColumn() &&
			rSnmpOidList[i].isCM() &&
			rSnmpOidList[i].isValid()
			) {

			string colKey;
			string oidId;
			string snmpValue;

			rSnmpOidList[i].getProperty(KEY_PARAM_COL_KEY, colKey);
			rSnmpOidList[i].getProperty(KEY_PARAM_OID_ID, oidId);

			getSnmpValue(oidId, snmpValue);

			// Add only non-empty values...
			if (snmpValue.size() > 0) {
				row.addColumn(colKey, snmpValue);
			}

		}

	}

}

/**
 * Do some actions using CM Status.
 *
 * If status is not active ifs should be = 0
 *
 */
void CCmtsCm::doCmStatusActions() {

	if (this->isActiveVisible() == false) {

		//this->setParam(KEY_COL_CM_STATUS, STATUS_CM_OTHER_STR);
		//this->setCmIfUp("0");
		//this->setCmIfDwn("0");

	}

}

/**
 * Save everything needed from CM.
 * In current implementation save is into DB.
 *
 * TODO: Increase cohesion by making separate SnmpValues Save class (to deal with db and so on...)
 *
 */
bool CCmtsCm::save(CMySqlDB* pDb) {

	// Use DOCS3 SNMP Values...
	processSnmpValuesDocs3();

	// Do Not insert data for non active modems
	if (isActive()) {
	//if (this->isActiveVisible()) {

		sqlSaveIntoCmErrors(pDb);

		// Calculate CW Counters
		calcErrorsCounters();

		saveSnmpRow(pDb);
	} else {
		//cout << __FUNCTION__ << ":pid:" << getpid() << "cmts:" << getCmtsId() << " sid:" << m_cm_sid << "ip:" << getCmIp() << " Warning: CM is not Active" << endl;

		if (isActive()) {
			cout << __FUNCTION__ << ":pid:" << getpid();
			cout << "cmts:" << getCmtsId() << " sid:" << getCmSid() << " ip:" << getCmIp() << " mac:" << getCmMac();
			cout << " Warning: CM is not Saved: isActive:" << isActive();
			cout << " isParamDwnSnrValid:" << m_snmp_value_set.isParamDwnSnrValid();
			cout << endl;
		}

	}

	this->doCmStatusActions();

	sqlSaveIntoCmList(pDb);

	// TODO: Not sure should we update every Docs3 CM...
	sqlSaveIntoCmListDocs3(pDb);

	sqlSaveIntoCmCpeList(pDb);

	return true;
}

/**
 * Save all Snmp Values as single Row (1 insert into db for ex.)
 */
bool CCmtsCm::saveSnmpRow(CMySqlDB* pDb) {

	sqlSaveIntoCmLatest(pDb);

	sqlInsertSnmpRow(pDb);

	sqlInsertSnmpRowDocs3(pDb);

	return true;
}

/**
 * Process Docsis 3 SnmpValues.
 * Replace/Add SNR in CM Lates with the lowest SNR values for a interface.
 * Replace/Add other params according to already found IfIDs.
 */
bool CCmtsCm::processSnmpValuesDocs3() {

	string colKey;
	string ifChn;
	string ifUp;
	string ifDwn;

	//cout << __FUNCTION__ << ":pid:" << getpid() << " sid:" << m_cm_sid << endl;

	// First SnmpOid Pass to find ifIds....
	for (int i = 0; i < m_snmp_value_set_docs3.size(); i++) {

		CSnmpValueDocs3& rSnmpValueDocs3 = *(m_snmp_value_set_docs3.get(i));

		const CSnmpOid& rSnmpOid = rSnmpValueDocs3.getSnmpOid();

		rSnmpOid.getProperty(KEY_PARAM_COL_KEY, colKey);
		rSnmpOid.getProperty(KEY_PARAM_CHANNEL, ifChn);

		CSnmpValue* pSnmpValue = m_snmp_value_set.getSnmpValue(colKey);

		if (rSnmpOid.isDimdB() && rSnmpOid.hasColumn()) {

			CSnmpValue* pMinSnmpVal = rSnmpValueDocs3.getMinSnmpValue();

			if (pMinSnmpVal != NULL) {

				string ifId;

				rSnmpValueDocs3.getIfId(pMinSnmpVal, ifId);

				if (pSnmpValue == NULL) {
					setSnmpValue(rSnmpOid, pMinSnmpVal->getRawValue());
				} else {
					pSnmpValue->setSnmpValueParsed(pMinSnmpVal->getValue());
				}

				if (ifChn.compare(CHANNEL_VAL_UP) == 0) {
					setCmIfUp(ifId);
					ifUp = ifId;
				} else if (ifChn.compare(CHANNEL_VAL_DOWN) == 0) {
					setCmIfDwn(ifId);
					ifDwn = ifId;
				}

			} else {
				cout << __FUNCTION__ << ":" << __LINE__ << ":pid:" << getpid() << " sid:" << getCmSid();
				cout << " " << colKey << " ERROR:Min SnmpVal Not Found:" << endl;
			}

		}

		// Second Pass to write other params according to found IfIDs
		for (int i = 0; i < m_snmp_value_set_docs3.size(); i++) {

			CSnmpValueDocs3& rSnmpValueDocs3 = *(m_snmp_value_set_docs3.get(i));

			const CSnmpOid& rSnmpOid = rSnmpValueDocs3.getSnmpOid();

			rSnmpOid.getProperty(KEY_PARAM_COL_KEY, colKey);
			rSnmpOid.getProperty(KEY_PARAM_CHANNEL, ifChn);

			CSnmpValue* pSnmpValue = m_snmp_value_set.getSnmpValue(colKey);

			if (rSnmpOid.isDimdB() == false && rSnmpOid.hasColumn()) {

				string foundIfId;

				CSnmpValue* pSnmpValueFoundMin;

				if (ifChn.compare(CHANNEL_VAL_UP) == 0) {
					foundIfId = ifUp;
				} else if (ifChn.compare(CHANNEL_VAL_DOWN) == 0) {
					foundIfId = ifDwn;
				}

				pSnmpValueFoundMin = rSnmpValueDocs3.getSnmpValue(foundIfId);

				if (pSnmpValueFoundMin != NULL) {
					if (pSnmpValue == NULL) {
						setSnmpValue(rSnmpOid, pSnmpValueFoundMin->getRawValue());
					} else {
						pSnmpValue->setSnmpValueParsed(pSnmpValueFoundMin->getValue());
					}
				} else {
					//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":pid:" << getpid() << " sid:" << m_cm_sid;
					//cout << " " << colKey << " ERROR:pSnmpValueFoundMin Not Found:" << endl;
				}
			}

		}

	}

	return true;
}

/**
 * Insert into Snmp Function Table single Row with all Snmps
 */
bool CCmtsCm::sqlInsertSnmpRow(CMySqlDB* pDb) {

	string sqlInsert;

	getSqlInsertSnmpRow(sqlInsert);
	//cout << __FUNCTION__ << "SQL: " << sqlInsert << endl;
	pDb->doInsert(sqlInsert);

	getSqlInsertSnmpRowToday(sqlInsert);
	//cout << __FUNCTION__ << "SQL: " << sqlInsert << endl;
	try {
		pDb->doInsert(sqlInsert);
	} catch (CErrorSql& e) {
		cout << "ERROR: getSqlInsertSnmpRowToday: " << e.getMessage() << endl;
	}


	return true;
}

bool CCmtsCm::sqlSaveIntoCmCpeList(CMySqlDB* pDb) {

	unsigned int i;

	vector<string> sqlInsert;

	// Skip if no CPEs
	if (m_cpe_list.size() == 0) {
		return false;
	}

	getSqlSaveIntoCmCpeList(sqlInsert);

	for(i = 0; i < sqlInsert.size(); i++) {

		//cout << __FUNCTION__ << ":pid:" << getpid() << " cmts:" << getCmtsId();
		//cout << " cm:" << getCmMac() << " SQL[" << i << "]: " << sqlInsert[i] << endl;

		pDb->doExecute(sqlInsert[i]);

	}

	return true;
}

bool CCmtsCm::sqlInsertSnmpRowDocs3(CMySqlDB* pDb) {

	unsigned int i;

	vector<string> sqlInsert;

	// Skip if no Docs3 SnmpValues
	if (m_snmp_value_set_docs3.size() == 0) {
		return false;
	}

	getSqlInsertSnmpRowDocs3(sqlInsert);

	for(i = 0; i < sqlInsert.size(); i++) {

		//cout << __FUNCTION__ << ":pid:" << getpid() << " cmts:" << getCmtsId();
		//cout << " cm:" << getCmMac() << " SQL[" << i << "]: " << sqlInsert[i] << endl;

		pDb->doExecute(sqlInsert[i]);

	}

	return true;
}

/**
 * Make Sql statement for INSERT/UPDATE of CM List table
 * with some of the main params (mac, ip, status...)
 * TODO: Make additional params to be added auto (like create cm list table aux params)
 */
void CCmtsCm::getSqlSaveIntoCmList(string& result) {

	string ifDwn = getParam(KEY_SNMP_OID_CM_CH_DWN);
	string ifUp = getParam(KEY_SNMP_OID_CM_CH_UP);
	string cmIp = getCmIp();
	string cmStat = getParam(KEY_COL_CM_STATUS);

	CDataRow rowAuxColumns;
	string auxCol;
	string auxVal;
	string auxColVal;

	time_t dumpDate = time(NULL);

	stringstream ss;

	// Validation of CM Params
	if (ifDwn.size() == 0) {
		ifDwn = "0";
	}
	if (ifUp.size() == 0) {
		ifUp = "0";
	}
	if (cmIp.size() == 0) {
		cmIp = "0";
	}
	if (cmStat.size() == 0) {
		cmStat = "1";
	}

	doCmListAuxColumns(rowAuxColumns);
	//rowAuxColumns.dumpInfo();

	rowAuxColumns.getColumnNamesCSV(auxCol);
	rowAuxColumns.getColumnValuesCSV(auxVal, "\'");
	rowAuxColumns.getColumnsCSV(auxColVal, "\'");

	ss << "INSERT INTO " << TABLE_OSS_CM_LIST;
	ss << " (cm_mac, cmts_id, cm_sid, cm_ip, cm_status, date_list, if_dwn, if_up";

	if (auxCol.size() > 0) {
		ss << ", " << auxCol;
	}

	ss << " ) ";
	ss << " VALUES( ";
	ss << this->getCmMac() << string(", ");
	ss << getCmtsId() << ", ";
	ss << getCmSid() << ", ";
	ss << " '" << cmIp << "', ";
	ss << cmStat << ", ";
	ss << dumpDate << ", ";
	ss << ifDwn << ", ";
	ss << ifUp;

	if (auxVal.size() > 0) {
		ss << ", " << auxVal;
	}

	ss << ") ";
	ss << " ON DUPLICATE KEY UPDATE ";
	ss << " cm_sid = " << getCmSid() << ", ";
	ss << " cm_ip = '" << cmIp << "', ";
	ss << " cm_status = " << cmStat << ", ";
	ss << " date_list = " << dumpDate << ", ";
	ss << " if_dwn = " << ifDwn << ", ";
	ss << " if_up = " << ifUp;

	if (auxColVal.size() > 0) {
		ss << ", " << auxColVal;
	}

	result = ss.str();

}

/**
 * Make Sql statement for INSERT/UPDATE of CM CPE List.
 */
void CCmtsCm::getSqlSaveIntoCmCpeList(vector<string>& result) {

	time_t dumpDate = time(NULL);

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << " CM:" << getCmIp() << endl;

	result.clear();

	for(int i = 0; i < m_cpe_list.size(); i++) {

		stringstream sqlResult;

		sqlResult << "INSERT INTO " << TABLE_OSS_CM_CPE_LIST;
		sqlResult << "(cmts_id, cm_mac, dumpdate, cpe_mac, cpe_ip) ";
		sqlResult << "VALUES (";
		sqlResult << getCmtsId() << ", " << getCmMac() << ", ";
		sqlResult << dumpDate << ", ";
		sqlResult << "\'" << m_cpe_list.getCpe(i).getMac() << "\', \'" << m_cpe_list.getCpe(i).getIp() << "\') ";

		result.push_back(sqlResult.str());
	}

}

/**
 * Make Sql statement for INSERT/UPDATE of latest snmp values row into CM Latest table.
 */
void CCmtsCm::getSqlSaveIntoCmLatest(string& result) {

	stringstream sqlColumns;
	stringstream sqlValues;
	stringstream sqlUpdates;

	time_t dumpDate = time(NULL);

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << " CM:" << getCmIp() << endl;

	result = "INSERT INTO "TABLE_OSS_CM_LATEST;

	CSnmpValue* pSnmpValue;

	resetSnmpValueIterator();

	while (getNextSnmpValueFnCol(&pSnmpValue)) {

		CSnmpValue& rSnmpValue = *pSnmpValue;
		const CSnmpOid& rSnmpOid = pSnmpValue->getSnmpOid();

		string rColKey;
		const string& rColVal = rSnmpValue.getValue();

		rSnmpOid.getProperty(KEY_PARAM_COL_KEY, rColKey);

		sqlColumns << rColKey << ", ";
		sqlValues << rColVal << ", ";

		sqlUpdates << rColKey << " = " << rColVal << ", ";
	}

	// Add cmts/mac ids and date
	sqlColumns << "cmts_id, cm_mac, dumpdate";
	sqlValues << getCmtsId() << ", " << getCmMac() << ", " << dumpDate;

	sqlUpdates << "dumpdate = " << dumpDate;

	result += " (" + sqlColumns.str() + ") ";
	result += " VALUES (" + sqlValues.str() + ") ";

	result += " ON DUPLICATE KEY UPDATE ";
	result += sqlUpdates.str();

}

/**
 * Make Sql statement for INSERT/UPDATE of latest snmp values row into CM Errors table.
 */
void CCmtsCm::getSqlSaveIntoCmErrors(string& result) {

	stringstream sqlColumns;
	stringstream sqlValues;
	stringstream sqlUpdates;

	time_t dumpDate = time(NULL);

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << " CM:" << getCmIp() << endl;

	result = "INSERT INTO "TABLE_OSS_CM_ERRORS;

	CSnmpValue* pSnmpValue;

	resetSnmpValueIterator();

	while (getNextSnmpValueCW(&pSnmpValue)) {

		CSnmpValue& rSnmpValue = *pSnmpValue;
		const CSnmpOid& rSnmpOid = pSnmpValue->getSnmpOid();

		string rColKey;

		// if there is no column key, should be generated one...
		if (rSnmpOid.hasColumn() == false) {
			rColKey = rSnmpOid.getProperty(KEY_PARAM_CHANNEL) + "_unerrored";
		} else {
			rColKey = rSnmpOid.getProperty(KEY_PARAM_COL_KEY);
		}

		const string& rColVal = rSnmpValue.getValue();

		sqlColumns << rColKey << ", ";
		sqlValues << rColVal << ", ";

		sqlUpdates << rColKey << " = " << rColVal << ", ";

	}

	// Add cmts/mac ids
	sqlColumns << "cmts_id, cm_mac, dumpdate";
	sqlValues << getCmtsId() << ", " << getCmMac() << ", " << dumpDate;

 	sqlUpdates << "dumpdate = " << dumpDate;

	result += " (" + sqlColumns.str() + ") ";
	result += " VALUES (" + sqlValues.str() + ") ";

	result += " ON DUPLICATE KEY UPDATE ";
	result += sqlUpdates.str();

}

void CCmtsCm::prepareSqlInsertSnmpRow(string& result) {

	stringstream sqlColumns;
	stringstream sqlValues;

	CSnmpValue* pSnmpValue;

	time_t dumpDate = time(NULL);

	resetSnmpValueIterator();

	while (getNextSnmpValueFnCol(&pSnmpValue)) {

		CSnmpValue& rSnmpValue = *pSnmpValue;
		const CSnmpOid& rSnmpOid = pSnmpValue->getSnmpOid();

		sqlColumns << rSnmpOid.getProperty(KEY_PARAM_COL_KEY) << ", ";
		sqlValues << rSnmpValue.getValue() << ", ";

	}

	// Add cmts/mac ids and date
	sqlColumns << "cmts_id, cm_mac, dumpdate";
	sqlValues << getCmtsId() << ", " << getCmMac() << ", " << dumpDate;

	result += " (" + sqlColumns.str() + ") ";
	result += " VALUES (" + sqlValues.str() + ") ";
}

/**
 * TODO: Testing insert into common memory table.
 * Should delete older records.
 *
 */
void CCmtsCm::getSqlInsertSnmpRowToday(string& result) {

	result = "INSERT DELAYED INTO ";
	result += TABLE_NAME_PREFIX_OSS_CM;
	result += TABLE_NAME_POSTFIX_TODAY;

	this->prepareSqlInsertSnmpRow(result);

}

/**
 * Make Sql statement for INSERT of a single snmp values row.
 */
void CCmtsCm::getSqlInsertSnmpRow(string& result) {

	string tableCm = m_tools_sql.initCmTable();

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << " CM:" << getCmIp() << endl;

	result = "INSERT INTO " + tableCm;

	this->prepareSqlInsertSnmpRow(result);

}

/**
 * Do snmp get CM Status from CMTS.
 * Used for cm status on demand.
 * TODO: now the status oid param is made as a Aux param.
 *
 */
void CCmtsCm::doSnmpGetCmStatus(string& result) {

	vector<CSnmpValue> cmtsSnmpVal;

	vector<CSnmpOid> listAuxOids;

	unsigned int i;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	doCmCmtsSnmpListAux(listAuxOids);

	doCmtsSnmpGet(listAuxOids, cmtsSnmpVal);

	for (i = 0; i < cmtsSnmpVal.size(); i++) {
		//cmtsSnmpVal[i].dumpInfo();

		cmtsSnmpVal[i].dumpStrInfo(result);

		result += "\n";
	}

}

CCmtsCm::CCmtsCm(const CDataRow& row) {
	this->init(row);
}


/**
 * Initialize from DataRow.
 * Just copy all params from row to CM.
 *
 */
void CCmtsCm::init(const CDataRow& row) {

	for(int i = 0; i < row.size(); i++) {

		string key;
		string value;

		row.getColumn(i, key, value);

		this->setParam(key, value);

	}
}

bool CCmtsCm::isParamValid(const string& param) {
	return m_snmp_value_set.isParamValid(param);
}

void CCmtsCm::doCmtsSnmpGetDocs3(const vector<CSnmpOid>& listSnmpOids,
		CSnmpValueDocs3Set& snmpValueSet) {

	unsigned int i;
	unsigned int j;

	cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__;
	cout << " CMTS IP:" << getParam(KEY_SNMP_OID_CM_CMTS_IP) << " rcomm:" << getRCommunityCmts() << endl;

	CIp cmtsIp(getParam(KEY_SNMP_OID_CM_CMTS_IP), getRCommunityCmts());

	CDataSet ds;
	CDataRow row;

	snmpValueSet.clear();

	for (i = 0; i < listSnmpOids.size(); i++) {

		// Skip Non Docsis3 oids
		if (atoi(listSnmpOids[i].getProperty(KEY_PARAM_DOCSIS).c_str()) != 3) {
			continue;
		}
		// Skip Docsis3 params Not Needed - TODO: Consider manage via Oid Table Param.
		if (listSnmpOids[i].isDimdB() == false) {
			continue;
		}

		stringstream ssSnmpOid;

		ssSnmpOid << listSnmpOids[i].getSnmpOid() << "." << getParam(KEY_COL_CM_SID);

		cmtsIp.SnmpWalkBulkDocs3(ssSnmpOid.str(), getRCommunityCmts(), ds);

		CSnmpValueDocs3 snmpValue(listSnmpOids[i]);

		for(j = 0; j < ds.size(); j++) {
			ds.getRow(j, row);
			snmpValue.setSnmpValue(row.getColumn(KEY_IF_ID), row.getColumn(KEY_VALUE));
		}

		snmpValueSet.setDocs3SnmpValue(snmpValue);
	}
}

void CCmtsCm::dumpCmShortInfo() {
	cout << " macid:" << this->getCmMac();
	cout << " mac:" << this->getCmMacHex();
	cout << " cmip:" << this->getCmIp();
	cout << " cmtsId:" << this->getCmtsId();
}

/**
 * Should Validate the CM.
 * Different parameters could be used...
 * Most proper are: cmIp, cmMac ...
 */
bool CCmtsCm::isValid() {

	return this->isValidIp();
}


/**
 * Validate The CM IP.
 * Invalid IP can be 0, 0.0.0.0...
 * TODO: use regexp for ([0-255].)5[0-255]
 */
bool CCmtsCm::isValidIp() {

	bool result = true;

	string ip = this->getCmIp();

	// TODO: check here different conditions of invalid ip
	if (	ip.compare("0") == 0 ||
			ip.compare("0.0.0.0") == 0 ||
			ip.size() < 7) {

		result = false;
	}

	return result;
}

/**
 * Do a list of Docs3 Oids for direct CM query.
 *
 * TODO: Consider get the list from DB Table with cpe oid types.
 *
 */
void CCmtsCm::doCmSnmpListAuxDocs3(vector<CSnmpOid>& listSnmp) {

	CSnmpOid dwnSnrOid(SNMP_OID_CM_DOCS3_DS_SNR, 0);
	dwnSnrOid.addProperty(KEY_PARAM_OID_LABEL, "If3 Dwn SNR");
	dwnSnrOid.addProperty(KEY_PARAM_OID_DIM, KEY_PARAM_DIM_DB);
	dwnSnrOid.addProperty(KEY_PARAM_OID_MULT, "0.1");
	listSnmp.push_back(dwnSnrOid);

	/*
	CSnmpOid freqOid(SNMP_OID_CM_DOCS3_DS_FREQ, 1);
	freqOid.addProperty(KEY_PARAM_OID_LABEL, "If3 Dwn Freq");
	freqOid.addProperty(KEY_PARAM_OID_DIM, "MHz");
	freqOid.addProperty(KEY_PARAM_OID_MULT, "0.000001");
	listSnmp.push_back(freqOid);
*/

}

/**
 * Do a Docs3 Snmp Oids direct CM query.
 *
 */
void CCmtsCm::doCmSnmpGetDocs3(const vector<CSnmpOid>& listSnmpOids,
		CSnmpValueDocs3Set& setSnmpValues) {

	unsigned int i;
	unsigned int j;

	CIp cmIp(this->getCmIp(), getRCommunityCm());

	CDataSet ds;
	CDataRow row;

	setSnmpValues.clear();

	for (i = 0; i < listSnmpOids.size(); i++) {

		cmIp.SnmpWalkBulkDocs3(listSnmpOids[i].getSnmpOid(), getRCommunityCm(), ds);

		CSnmpValueDocs3 snmpValue(listSnmpOids[i]);

		for (j = 0; j < ds.size(); j++) {
			ds.getRow(j, row);
			snmpValue.setSnmpValue(row.getColumn(KEY_IF_ID), row.getColumn(KEY_VALUE));
		}

		setSnmpValues.setDocs3SnmpValue(snmpValue);
	}
}

/**
 * Return Upstream or Downstream of CM according to Snmp Oid channel.
 * Return single up/down stream, not using Docs3 if lists...
 */
string CCmtsCm::getCmIf(const CSnmpOid& oid) const {

	string cmIf;

	if (oid.isUpstreamStrict()) {
		cmIf = this->getParam(KEY_SNMP_OID_CM_CH_UP);
	} else if (oid.isDownstreamStrict()) {
		cmIf = this->getParam(KEY_SNMP_OID_CM_CH_DWN);
	}

	return cmIf;
}

/**
 * Check if the CM status is registrationComplete and should be visible via SNMP.
 *
 */
bool CCmtsCm::isActiveVisible() {
	return (isActive() && m_snmp_value_set.isParamDwnSnrValid());
}

/**
 * Set the CM IP using already set MAC and query current ip from cmts.
 */
void CCmtsCm::setCmIpFromMac() {

	CDataRow rowCmts;

	this->getParam(rowCmts);

	CSnmpCmIp snmpCm(rowCmts);

	if (snmpCm.getCmIp().size() > 0) {
		this->setCmIp(snmpCm.getCmIp());
	}
}

/**
 * Validate current CM IP.
 * If not valid try to find valid IP using other ways...
 *
 */
void CCmtsCm::setCmValidIp() {

	if (this->isValidIp() == false) {

		this->setCmIpFromMac();

	}

}

void CCmtsCm::getSqlInsertSnmpRowDocs3(vector<string>& result) {

	time_t dumpDate = time(NULL);


	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << " CM:" << getCmIp() << endl;

	//m_snmp_value_set_docs3.dumpInfo();

	if (m_snmp_value_set_docs3.size() > 0) {

		CSnmpValueDocs3* pSNRDocs3 = m_snmp_value_set_docs3.getDimDb();

		// Skip All if not found...
		if (pSNRDocs3 == NULL) {
			return;
		}

		// Iterate by IfId... TODO: Make proper if_id iteration...
		for(int i = 0; i < pSNRDocs3->size(); i++) {

			string ifId;

			pSNRDocs3->getIfId(i, ifId);

			string strSqlInsert;

			stringstream sqlColumns;
			stringstream sqlValues;

			strSqlInsert = "INSERT INTO " + m_tools_sql.getCmTableDocs3();

			for(int j = 0; j < m_snmp_value_set_docs3.size(); j++) {

				CSnmpValueDocs3* pSnmpDocs3 = m_snmp_value_set_docs3.get(j);

				// Skip if not found...
				if (pSnmpDocs3 == NULL) {
					continue;
				}

				CSnmpValue* pSnmpValue = pSnmpDocs3->getSnmpValue(ifId);

				// Skip if not found...
				if (pSnmpValue == NULL) {
					continue;
				}

				sqlColumns << pSnmpValue->getSnmpOid().getProperty(KEY_PARAM_COL_KEY) << ", ";
				sqlValues << pSnmpValue->getValue() << ", ";

			}

			// Add cmts/mac ids and date
			sqlColumns << "cmts_id, cm_mac, dumpdate, if_id";
			sqlValues << getCmtsId() << ", " << getCmMac() << ", ";
			sqlValues << dumpDate << ", " << ifId;

			strSqlInsert += " (" + sqlColumns.str() + ") ";
			strSqlInsert += " VALUES (" + sqlValues.str() + ") ";

			result.push_back(strSqlInsert);
		}

	}

}

/**
 * Insert into oss_cm_list already filled CM params.
 */
bool CCmtsCm::sqlSaveIntoCmList(CMySqlDB* pDb) {

	string sqlSave;

	getSqlSaveIntoCmList(sqlSave);

	//cout << __FUNCTION__ << ":SQL: " << sqlSave << endl;

	pDb->doExecute(sqlSave);

	return true;
}

bool CCmtsCm::sqlSaveIntoCmListDocs3(CMySqlDB* pDb) {

	CSnmpValueDocs3* snmpVal = m_snmp_value_set_docs3.getDimDb();

	if (snmpVal != NULL) {
		vector < string > &listIfId = snmpVal->getIfIdList();

		if (listIfId.size() > 1) {
			for (unsigned int i = 0; i < listIfId.size(); i++) {
				m_tools_sql.insertCmDocs3List(pDb, getCmMac(), listIfId[i]);
			}
		}
	}

	return true;
}

/**
 * Insert into oss_cm_latest Latest CM params
 */
bool CCmtsCm::sqlSaveIntoCmLatest(CMySqlDB* pDb) {

	string sqlSave;

	getSqlSaveIntoCmLatest(sqlSave);

	//cout << __FUNCTION__ << "SQL: " << sqlSave << endl;

	pDb->doExecute(sqlSave);

	return true;
}

/**
 * Insert into oss_cm_errors Latest CM Error Counters
 */
bool CCmtsCm::sqlSaveIntoCmErrors(CMySqlDB* pDb) {

	string sqlSave;

	getSqlSaveIntoCmErrors(sqlSave);

	pDb->doExecute(sqlSave);

	return true;
}

void CCmtsCm::initCmTable() {

	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	initCmTable(pDb);

	delete pDb;
}
/**
 * Initialize CM Table Name - used to store SNMP values.
 */
void CCmtsCm::initCmTable(CMySqlDB* pDb) {

	m_tools_sql.initCmTable(pDb);

}

void CCmtsCm::setCmIfUp(const string& ifUp) {
	setParam(KEY_SNMP_OID_CM_CH_UP, ifUp);
}

void CCmtsCm::setCmIfDwn(const string& ifDwn) {
	setParam(KEY_SNMP_OID_CM_CH_DWN, ifDwn);
}

/**
 * Set MAC from decimal converted MAC
 */
void CCmtsCm::setCmMacDecimal(const string& cm_mac_d) {

	setParam(KEY_SNMP_OID_CM_MAC_DEC, cm_mac_d);

	string cmMacHex;

	CStringUtil::convertDecimal2Mac(cm_mac_d, ":", cmMacHex);
	CStringUtil::formatMac(":", cmMacHex);
	setParam(KEY_SNMP_OID_CM_MAC_HEX, cmMacHex);
}
/**
 * Set MAC from hex MAC input.
 * Converts hex MAC to decimal
 */
void CCmtsCm::setCmMac(const string& cmMacRaw) {

	string cmMac = cmMacRaw;
	string cmMacDec;

	CStringUtil::formatMac(":", cmMac);
	setParam(KEY_SNMP_OID_CM_MAC_HEX, cmMac);

	CStringUtil::convertMac2Dec(cmMac, cmMacDec);
	this->setCmMacDecimal(cmMacDec);
}

string CCmtsCm::getCmSid() const {
	return getParam(KEY_COL_CM_SID);
}

void CCmtsCm::setCmSid(const string& cm_sid) {
	setParam(KEY_COL_CM_SID, cm_sid);
}

void CCmtsCm::setCmtsIpTools(CIp* ip_tools) {
	m_Ip = ip_tools;
}

void CCmtsCm::setCmIpTools(CIp* ip_tools) {
	m_pCmIp = ip_tools;
}

CIp& CCmtsCm::getCmIpTools() {
	return m_cmIp;
}

void CCmtsCm::initCmIpTools() {
	m_cmIp.setIp(getCmIp());
	m_cmIp.setCommunity(getRCommunityCm());
}
void CCmtsCm::initCmSnmpTools() {

	initCmIpTools();

	m_cmIp.InitSnmpSession(getRCommunityCm());
}
void CCmtsCm::closeCmSnmpSession() {
	m_cmIp.CloseSnmpSession();
}

string CCmtsCm::getCmMac() const {
	return this->getParam(KEY_SNMP_OID_CM_MAC_DEC);
}

string CCmtsCm::getCmMacHex() const {
	return this->getParam(KEY_SNMP_OID_CM_MAC_HEX);
}

void CCmtsCm::setCmIp(const string& ip) {
	setParam(KEY_COL_CM_IP, ip);
}
string CCmtsCm::getCmIp() const {
	return getParam(KEY_COL_CM_IP);
}

void CCmtsCm::setCmStatus(const string& status) {
	setParam(KEY_COL_CM_STATUS, status);
}
string CCmtsCm::getCmStatusStr() const {
	return getParam(KEY_COL_CM_STATUS);
}
int CCmtsCm::getCmStatus() const {
	return atoi(getCmStatusStr().c_str());
}

/**
 * Check if the collected params valid (if CM direct queried params are 0 is not a valid param set)
 * 
 */
bool CCmtsCm::isParamSetValid() {
	return m_snmp_value_set.isParamSetValid();
}

/**
 * Check if the CM is active(status is registrationComplete)
 */
bool CCmtsCm::isActive() {
	bool status;

	if (getParam(KEY_COL_CM_STATUS).compare(STATUS_CM_REGISTRATION_COMPLETE_STR) == 0) {
		status = true;
	} else {
		status = false;
	}

	return status;
}

/**
 * Validate Down SNR for the CM.
 * Should SNR be above the down snr threshold - red dwn snr < 35db
 */
bool CCmtsCm::isDwnSnrValid() {

	/*
	bool status = true;
	CSnmpValue* pSnmpValue = NULL;

	pSnmpValue = getSnmpValue(KEY_COL_KEY_DWN_SNR);

	if (pSnmpValue) {

		float fDwnSnr = pSnmpValue->getValueFloat();

		if(fDwnSnr >= 10) {
			status = true;
		} else {
			status = false;
		}

	} else {
		status = false;
	}

	return status;
*/
	return this->m_snmp_value_set.isParamDwnSnrValid();
}

/**
 * Initialize CM from DB, should cm_id(MAC in decimal) be avaiable.
 */
void CCmtsCm::initFromDb() {

	CDataRow rowCmtsInfo;
	CDataRow rowCmInfo;

	string cmtsIp;
	string cmtsRcomm;
	string cmRcomm;
	string cmIp;
	string cmSid;
	string cmStatus;

	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	m_tools_sql.sqlGetCmtsInfo(*pDb, getCmtsId(), rowCmtsInfo);
	m_tools_sql.sqlGetCmInfo(*pDb, getCmtsId(), this->getCmMac(), rowCmInfo);

	rowCmtsInfo.getColumn(KEY_COL_CMTS_IP, cmtsIp);
	rowCmtsInfo.getColumn(KEY_COL_CMTS_RCOMM, cmtsRcomm);
	rowCmtsInfo.getColumn(KEY_COL_CM_RCOMM, cmRcomm);

	rowCmInfo.getColumn(KEY_COL_CM_IP, cmIp);
	rowCmInfo.getColumn(KEY_COL_CM_SID, cmSid);
	rowCmInfo.getColumn(KEY_COL_CM_STATUS, cmStatus);

	setCmtsIp(cmtsIp);
	setRCommunity(cmtsRcomm, cmRcomm);

	setParam(KEY_SNMP_OID_CM_CMTS_IP, cmtsIp);
	setParam(KEY_COL_CM_IP, cmIp);
	setParam(KEY_COL_CM_SID, cmSid);
	setParam(KEY_COL_CM_STATUS, cmStatus);
	setParam(KEY_COL_CMTS_RCOMM, cmtsRcomm);
	setParam(KEY_COL_CM_RCOMM, cmRcomm);

	m_tools_sql.selectOidList(pDb, string(""), m_snmp_oid);
	//m_tools_sql.selectCmOidList(pDb, m_snmp_oid_cm);
	//m_tools_sql.selectCmtsOidList(pDb, m_snmp_oid_cmts);
	m_tools_sql.selectCmOnDemandOidList(pDb, m_snmp_oid_cm);
	m_tools_sql.selectCmtsOnDemandOidList(pDb, m_snmp_oid_cmts);

	delete pDb;
}

/**
 * Do Aux Snmp List for some additional CM params from CMTS.
 */
void CCmtsCm::doCmCmtsSnmpListAux(vector<CSnmpOid>& listSnmp) {

	CSnmpOid statusOid(SNMP_OID_CM_STATUS, 0);
	statusOid.addProperty(KEY_PARAM_OID_LABEL, KEY_COL_CM_STATUS);
	statusOid.addProperty(KEY_PARAM_OID_DIM, "");
	listSnmp.push_back(statusOid);

}

/**
 * Get All Snmp Values defined for this CM.
 * Using Snmp Get from CMTS and CM.
 * Used main for SNMP on Demand
 */
void CCmtsCm::doSnmpGetAll(string& result) {

	vector<CSnmpValue> cmtsSnmpVal;
	vector<CSnmpValue> cmSnmpVal;
	CSnmpValueDocs3Set cmCmtsDocs3Set;
	CSnmpValueDocs3Set cmDocs3Set;

	vector<CSnmpOid> listAuxOids;
	vector<CSnmpOid> listAuxOidsCmDocs;

	unsigned int i;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	doCmCmtsSnmpListAux(listAuxOids);
	doCmSnmpListAuxDocs3(listAuxOidsCmDocs);

	doCmtsSnmpGet(listAuxOids, cmtsSnmpVal);
	doCmtsSnmpGet(m_snmp_oid_cmts, cmtsSnmpVal);

	doCmtsSnmpGetDocs3(m_snmp_oid, cmCmtsDocs3Set);
	doCmSnmpGetDocs3(listAuxOidsCmDocs, cmDocs3Set);

	doCmSnmpGet(m_snmp_oid_cm, cmSnmpVal);

	result += "mac: " + getParam("mac");
	result += " ip: " + getParam(KEY_COL_CM_IP) + "\n";

	result += "Snmp from CMTS\n\n";

	for(i = 0; i < cmtsSnmpVal.size(); i++) {
		//cmtsSnmpVal[i].dumpInfo();

		cmtsSnmpVal[i].dumpStrInfo(result);

		result += "\n";
	}

	// CM Docsis3 Params from CMTS
	cmCmtsDocs3Set.dumpSnmpValueListInfo("\n", result);

	result += "\nSnmp from CM\n\n";

	string resultNonFunction;

	for (i = 0; i < cmSnmpVal.size(); i++) {
		//cmSnmpVal[i].dumpInfo();

		if (m_snmp_oid_cm[i].isFunction()) {

			cmSnmpVal[i].dumpStrInfo(result);

			result += "\n";

		} else {

			cmSnmpVal[i].dumpStrInfo(resultNonFunction);

			resultNonFunction += "\n\n";

		}
	}

	// CM Docsis3 Params from CM
	cmDocs3Set.dumpSnmpValueListInfo("\n", result);

	CStringUtil strUtil;

	strUtil.replace(resultNonFunction, '<', ' ');
	strUtil.replace(resultNonFunction, '>', ' ');
	//strUtil.replace(resultNonFunction, '_', ' ');	

	result += "\n" + resultNonFunction;

}

/**
 * Get CM Speed info (in/out interface counters - dCount/dt)
 * TODO: Consider rework - not well done...
 */
void CCmtsCm::doSnmpGetCmSpeed(string& result) {

	vector<CSnmpOid> listOids;
	vector<CSnmpValueSpeed> cmSnmpVal;

	int period = 1; // Period between 2 snmp values

	unsigned int i;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	// Init in/out snmp oids... TODO: initialize from db...
	CSnmpOid oidIfIn("1.3.6.1.2.1.2.2.1.10.2", 0);
	oidIfIn.addProperty(KEY_PARAM_OID_ID, "66");
	oidIfIn.addProperty(KEY_PARAM_OID_LABEL, "Download Speed");
	oidIfIn.addProperty(KEY_PARAM_OID_DIM, "Mbit");
	oidIfIn.addProperty("oid_mult", "0.00000762939453125");

	CSnmpOid oidIfOut("1.3.6.1.2.1.2.2.1.16.4", 0);
	oidIfIn.addProperty(KEY_PARAM_OID_ID, "67");
	oidIfOut.addProperty(KEY_PARAM_OID_LABEL, "Upload Speed");
	oidIfOut.addProperty(KEY_PARAM_OID_DIM, "Mbit");
	oidIfOut.addProperty("oid_mult", "0.00000762939453125");

	listOids.push_back(oidIfIn);
	listOids.push_back(oidIfOut);

	initCmIpTools();

	for(i = 0; i < listOids.size(); i++) {

		CSnmpValueSpeed snmpVal(listOids[i]);

		snmpVal.setIpTools(&this->m_cmIp);
		snmpVal.setPeriod(period);

		snmpVal.start();

		cmSnmpVal.push_back(snmpVal);
	}

	for(i = 0; i < cmSnmpVal.size(); i++) {
		result += cmSnmpVal[i].getSpeed();
		result += "\n";
	}

}

/**
 * Do Snmp GET from CMTS for CM. Used for snmp get on demand usualy.
 * CM should be well configured already - has SID, read community...
 */
void CCmtsCm::doCmtsSnmpGet(const vector<CSnmpOid>& listSnmpOids, vector<CSnmpValue>& listSnmpValues) {

	string snmpResult;
	unsigned int i;

	cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;
	cout << "CMTS IP:" << getParam(KEY_SNMP_OID_CM_CMTS_IP) << " rcomm:" << getRCommunityCmts() << endl;

	CIp cmtsIp(getParam(KEY_SNMP_OID_CM_CMTS_IP), getRCommunityCmts());

	for (i = 0; i < listSnmpOids.size(); i++) {

		stringstream ssSnmpOid;

		ssSnmpOid << listSnmpOids[i].getSnmpOid() << "." << getParam(KEY_COL_CM_SID);

		cmtsIp.SnmpGet(ssSnmpOid.str(), snmpResult);

		CSnmpValue snmpValue(listSnmpOids[i], snmpResult);

		listSnmpValues.push_back(snmpValue);

		cout << "oid:" << ssSnmpOid.str() << " res:" << snmpResult << endl;
	}

}

/**
 * Do Snmp GET from CM. Used for snmp get on demand usualy.
 */
void CCmtsCm::doCmSnmpGet(vector<CSnmpOid>& listSnmpOids, vector<CSnmpValue>& listSnmpValues) {

	string snmpResult;
	unsigned int i;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	if (this->isValidIp() == false) {
		throw (CErrorCommon("Invalid IP: " + this->getCmIp()));
	}

	initCmIpTools();

	for(i = 0; i < listSnmpOids.size(); i++) {

		// Skip non-function snmp
/*
		if (listSnmpOids[i].isFunction() == false) {
			continue;
		}
*/

		m_cmIp.SnmpGet(listSnmpOids[i].getSnmpOid(), snmpResult);

		CSnmpValue snmpValue(listSnmpOids[i], snmpResult);

		listSnmpValues.push_back(snmpValue);

	}

}

void CCmtsCm::dumpCmCpeList() {

	cout << endl << "--------------------------" << endl;
	cout << "PID: " << getpid() << " ";
	cout << "MAC[" << this->getCmMac() << "] ";
	cout << "SID[" << getCmSid() << "] ";
	cout << endl;

	for(int i = 0; i < m_cpe_list.size(); i++) {
		cout << "\t" << i << ": " << m_cpe_list.getCpe(i).getMac();
		cout << m_cpe_list.getCpe(i).getIp() << endl;
	}
}

/**
 * Dump CM Info (for Debug)
 */
void CCmtsCm::dumpCmInfo() {
	cout << endl << "--------------------------" << endl;

	cout << "PID: " << getpid() << " ";
	cout << "MACS[" << this->getCmMac() << "] ";
	cout << "SID[" << getCmSid() << "] ";
	cout << endl;

	this->m_row_params.dumpInfo();

	m_snmp_value_set.dumpInfo();

	m_snmp_value_set_docs3.dumpInfo();

	cout << endl << "--------------------------" << endl;

}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */
