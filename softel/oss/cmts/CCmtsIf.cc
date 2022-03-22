/*
 * CCmtsIf.cc
 *
 *  Created on: 13.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCmtsIf.h"

#include <softel/core/error/CErrorSql.h>
#include <softel/core/error/CErrorCommon.h>

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::NET;
using namespace SF::CORE::UTIL;

namespace SF {
namespace OSS {
namespace CMTS {


CCmtsIf::CCmtsIf() {
	this->setType(-1);
}

CCmtsIf::CCmtsIf(const int cmts_id) { 
	setCmtsId(cmts_id);
	this->setType(-1);
}

CCmtsIf::CCmtsIf(const string& cmts_id) {
	setCmtsId(cmts_id);
	this->setType(-1);
}

CCmtsIf::CCmtsIf(const string& cmts_id, const string& if_id) {
	this->setCmtsId(cmts_id);
	this->setIfId(if_id);
	this->setType(-1);
}

CCmtsIf::~CCmtsIf() {

}

/**
 * Initialize IF from DB, should if_id and cmts_id are avaialble
 */
void CCmtsIf::initFromDb() {

	CDataRow rowCmtsInfo;
	CDataRow rowIfInfo;

	string cmtsIp;
	string cmtsRcomm;
	string ifId;
	string ifType;

	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	m_tools_sql.sqlGetCmtsInfo(*pDb, getCmtsId(), rowCmtsInfo);

	rowCmtsInfo.getColumn(KEY_COL_CMTS_IP, cmtsIp);
	rowCmtsInfo.getColumn(KEY_COL_CMTS_RCOMM, cmtsRcomm);

	setParam(KEY_COL_CMTS_IP, cmtsIp);
	setParam(KEY_COL_CMTS_RCOMM, cmtsRcomm);

	m_tools_sql.selectOidList(pDb, string(""), m_snmp_oid);
	m_tools_sql.selectCmtsIfOnDemandOidList(pDb, m_snmp_oid_cmts_if);

	ifId = getParam(KEY_SNMP_OID_IF_ID);
	m_tools_sql.sqlGetIfInfo(*pDb, getCmtsId(), ifId, rowIfInfo);

	rowIfInfo.getColumn(KEY_COL_IF_TYPE, ifType);
	setParam(KEY_SNMP_OID_IF_TYPE, ifType);
	m_type = atoi(ifType.c_str());

	delete pDb;
}

/**
 * Get Custom CMTS IF Snmp Param by Oid.
 * Using Snmp Get from CMTS.
 * Used main for SNMP on Demand
 *
 * TODO: Consider make SnmpOid List indexed representation
 * and make current method using list of oidId searched in the full oid list.
 */
void CCmtsIf::doSnmpGetCustomOid(const std::string& oidId, std::string& result) {

	vector<CSnmpOid> listSnmpOids;
	vector<CSnmpValue> cmtsSnmpVal;

	size_t i = 0;

	// Do current Snmp Oids list from all IF SnmpOids
	// TODO: Rework using SnmpOid List representation with indexing...
	for (i = 0; i < m_snmp_oid_cmts_if.size(); i++) {

		CSnmpOid& rSnmpOid = m_snmp_oid_cmts_if[i];

		if (	isOidSameStream(rSnmpOid)
			&& ( ((rSnmpOid.isAvgFromCM() == false) && (rSnmpOid.isFunction()) )
			|| rSnmpOid.isIfOnDemand())) {

			if (rSnmpOid.getOidIdStr().compare(oidId) == 0) {
				listSnmpOids.push_back(rSnmpOid);
			}
		}
	}

	doCmtsSnmpGet(listSnmpOids, cmtsSnmpVal);

	for (i = 0; i < cmtsSnmpVal.size(); i++) {
		cmtsSnmpVal[i].toFnPointXml(result);
		result += "\n";
	}
}

/**
 * Get All Snmp Values defined for this IF.
 * Using Snmp Get from CMTS.
 * Used main for SNMP on Demand
 */
void CCmtsIf::doSnmpGetAll(string& result) {

	vector<CSnmpOid> listSnmpOids;
	vector<CSnmpOid> listAuxOids;

	vector<CSnmpValue> cmtsSnmpVal;

	unsigned int i;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	listSnmpOids.clear();
	listAuxOids.clear();

	doCmtsIfSnmpListAux(listAuxOids);

	// Do current Snmp Oids list from all IF SnmpOids
	for(i = 0; i < m_snmp_oid_cmts_if.size(); i++) {

		CSnmpOid& rSnmpOid = m_snmp_oid_cmts_if[i];

		if (	isOidSameStream(rSnmpOid) &&
				( 		(rSnmpOid.isAvgFromCM() == false) && (rSnmpOid.isFunction())
						|| rSnmpOid.isIfOnDemand()
				)
		) {

			listSnmpOids.push_back(rSnmpOid);

		}
	}

	doCmtsSnmpGet(listSnmpOids, cmtsSnmpVal);
	doCmtsSnmpGet(listAuxOids, cmtsSnmpVal);

	result += "Snmp from CMTS\n\n";

	for(i = 0; i < cmtsSnmpVal.size(); i++) {
		//cmtsSnmpVal[i].dumpInfo();

		cmtsSnmpVal[i].dumpStrInfo(result);

		result += "\n";

	}

}

/**
 * Do Snmp GET from CMTS. Used for snmp get on demand usualy.
 * IF should be well configured already - has if id, cmts ip, read community...
 */
void CCmtsIf::doCmtsSnmpGet(const vector<CSnmpOid>& listSnmpOids, vector<CSnmpValue>& listSnmpValues) {

	string snmpResult;
	unsigned int i;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;
	//cout << "CMTS IP:" << getParam(KEY_SNMP_OID_CM_CMTS_IP) << " rcomm:" << getParam(KEY_SNMP_OID_CMTS_RCOMM) << endl;

	CIp cmtsIp(getParam(KEY_SNMP_OID_CM_CMTS_IP), getParam(KEY_COL_CMTS_RCOMM));

	for (i = 0; i < listSnmpOids.size(); i++) {

		stringstream ssSnmpOid;

		ssSnmpOid << listSnmpOids[i].getSnmpOid() << "." << getParam(KEY_SNMP_OID_IF_ID);

		cmtsIp.SnmpGet(ssSnmpOid.str(), snmpResult);

		CSnmpValue snmpValue(listSnmpOids[i], snmpResult);

		listSnmpValues.push_back(snmpValue);

		//cout << "oid:" << ssSnmpOid.str() << " res:" << snmpResult << endl;
	}

}

void CCmtsIf::setModProfilesList(CCmtsModProfilesList* pMp) {
	m_pModProfiles = pMp;
}

/**
 * Check if the interface is upstream or downstream.
 * Type should be 128 or 129.
 *
 */
bool CCmtsIf::isStream() {

	if (getType() == KEY_CHN_DOWNSTREAM || getType() == KEY_CHN_UPSTREAM) {
		return true;
	} else {
		return false;
	}

}

/**
 * Check if the interface is upstream
 */
bool CCmtsIf::isUpstream() {
	if (getType() == KEY_CHN_UPSTREAM) {
		return true;
	} else {
		return false;
	}
}

/**
 * Check if the interface is downstream
 */
bool CCmtsIf::isDownstream() {
	if (getType() == KEY_CHN_DOWNSTREAM) {
		return true;
	} else {
		return false;
	}
}

/**
 * Check if the stream channel is equal for the interface and the passed oid.
 *
 */
bool CCmtsIf::isOidSameStream(const CSnmpOid& oid) {
 
	if ( (isDownstream() && oid.isDownstream()) || (isUpstream() && oid.isUpstream()) ) {
		return true;
	} else {
		return false;
	}
}

/**
 * Check if the stream channel is equal for the interface and the passed oid.
 *
 */
bool CCmtsIf::isOidSameStreamStrict(const CSnmpOid& oid) {

	if ( (isDownstream() && oid.isDownstreamStrict()) || (isUpstream() && oid.isUpstreamStrict()) ) {
		return true;
	} else {
		return false;
	}
}

void CCmtsIf::setTypeUpstream() {
	setType(KEY_CHN_UPSTREAM);
}

void CCmtsIf::setTypeDownstream() {
	setType(KEY_CHN_DOWNSTREAM);
}

/**
 * Set Type of IF
 */
void CCmtsIf::setType(int type) {

	m_type = type;

	stringstream ssType;

	ssType << type;

	setParam(KEY_SNMP_OID_IF_TYPE, ssType.str());
}

/**
 * Return Type of interface.
 * If it is not extracted yet it will be.
 */
int CCmtsIf::getType() {

	if (m_type == -1) {

		string ifType = getParam(KEY_SNMP_OID_IF_TYPE);

		int nIfType;

		unsigned int posBegin = ifType.find_first_of('(');
		unsigned int posEnd = ifType.find_first_of(')');

		if (	posBegin > 0 && posBegin < ifType.size() &&
				posEnd > posBegin && posEnd < ifType.size()) {

			nIfType = atoi(ifType.substr(posBegin + 1, posEnd - posBegin - 1).c_str());

		} else {

			nIfType = -1;

		}

		m_type = nIfType;
	}

	return m_type;
}

/**
 * Increment Active CMs Number.
 *
 */
void CCmtsIf::incActiveCM(CSnmpOid& snmpOid) {

	int numActive = this->getActiveCmCount();

	numActive++;

	stringstream ss;
	ss << numActive;

	setSnmpValue(snmpOid, ss.str());
}

/**
 * Make list of columns for IF List.
 * Should response to: where is_fn = 0 and CHAR_LENGTH(col_key) > 0 and from_cm = 2 and prio < 100
 *
 */
void CCmtsIf::doIfListAuxColumns(CDataRow& row) {

	unsigned int i;

	vector<CSnmpOid>& rSnmpOidList = *m_pSnmpOidList;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << " oidSize:" << rSnmpOidList.size() << endl;

	for(i = 0; i < rSnmpOidList.size(); i++) {

		// Filter oids according to list requirements
		if (	rSnmpOidList[i].isFunction() == false &&
			rSnmpOidList[i].hasColumn() &&
			rSnmpOidList[i].isIf() &&
			rSnmpOidList[i].isValid()
			) {

			string colKey;
			string oidId;
			string snmpValue;

			rSnmpOidList[i].getProperty(KEY_PARAM_COL_KEY, colKey);
			rSnmpOidList[i].getProperty(KEY_PARAM_OID_ID, oidId);
			getSnmpValue(oidId, snmpValue);


			// Add Some Value Processing....
			// TODO: Some hardcoded changes here...
			if (colKey.compare(KEY_COL_UP_MODULATION) == 0) {
				int modProfile = atoi(snmpValue.c_str());
				m_pModProfiles->getDefaultModulation(modProfile, snmpValue);
			}

			row.addColumn(colKey, snmpValue);

		}

	}

}

/**
 * Save Everything
 *
 * TODO: Increase cohesion by making separate SnmpValues Save class (to deal with db and so on...)
 *
 */
bool CCmtsIf::save(CMySqlDB* pDb) {

	sqlSaveIntoIfList(pDb);

	sqlSaveIntoIfErrors(pDb);
	// Calculate CW Counters
	calcErrorsCounters();

	saveSnmpRow(pDb);

	return true;
}

/**
 * Save all Snmp Values as single Row (1 insert into db for ex.)
 */
bool CCmtsIf::saveSnmpRow(CMySqlDB* pDb) {

	sqlSaveIntoIfLatest(pDb);

	sqlInsertSnmpRow(pDb);

	return true;
}

/**
 * Make Sql statement for INSERT/UPDATE of latest snmp values row into CM Errors table.
 */
void CCmtsIf::getSqlSaveIntoIfErrors(string& result) {

	stringstream sqlColumns;
	stringstream sqlValues;
	stringstream sqlUpdates;

	time_t dumpDate = time(NULL);

	string ifId = getParam(KEY_SNMP_OID_IF_ID);

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << " CM:" << getCmIp() << endl;

	result = "INSERT INTO "TABLE_OSS_IF_ERRORS;

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
	sqlColumns << "cmts_id, if_id, dumpdate";
	sqlValues << getCmtsId() << ", " << ifId << ", " << dumpDate;

 	sqlUpdates << "dumpdate = " << dumpDate;

	result += " (" + sqlColumns.str() + ") ";
	result += " VALUES (" + sqlValues.str() + ") ";

	result += " ON DUPLICATE KEY UPDATE ";
	result += sqlUpdates.str();

}

/**
 * Get SNR Snmp Value for the interface.
 * Should conform to stream type.
 */
CSnmpValue* CCmtsIf::getValueSnr() {

	CSnmpValue* pSnmpValue = NULL;

	if (isUpstream()) {
		pSnmpValue = m_snmp_value_set.getSnmpValue(string(CHANNEL_VAL_UP) + string(KEY_COL_KEY_UP_SNR));
	} else if (isDownstream()) {
		pSnmpValue = m_snmp_value_set.getSnmpValue(string(CHANNEL_VAL_DOWN) + string(KEY_COL_KEY_DWN_SNR));
	}

	return pSnmpValue;
}

/**
 * Check if the IF Snmp Param set is valid.
 * Use SnmpValueSet param set validation
 * For Docsis streams - it checks only if there are snr params and if they are = 0.
 * For Ethernet - should has some activity on ifin/ifout
 * For other types - valid by default
 */
bool CCmtsIf::isIfParamSetValid() {
	
	bool status = true;

	if (this->isStream()) {
		status = this->isSnrActive();
	} else if (this->isEthernet()) {
		status = this->isInOutActive();
	} else {
		status = true;
	}
	
	return status;
}

/**
 * Make SQL to SAVE IF into IF LIST.
 * TODO: Increase cohesion by Moving it to specific class.
 */
void CCmtsIf::getSqlSaveIntoIfList(string& result) {

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	stringstream ss;

	string ifId = getParam(KEY_SNMP_OID_IF_ID);
	string ifType = getParam(KEY_SNMP_OID_IF_TYPE);
	string ifName = getParam(KEY_SNMP_OID_IF_NAME);
	string ifDesc = getParam(KEY_SNMP_OID_IF_DESC);
	//string ifAlias = getParam(KEY_SNMP_OID_IF_ALIAS);

	CDataRow rowAuxColumns;
	string auxCol;
	string auxVal;
	string auxColVal;

	int nIfType = getType();

	ss.clear();

	// Validation of Params
	if (atoi(ifId.c_str()) <= 0) {
		return;
	}
	if (nIfType <= 0) {
		return;
	}

	doIfListAuxColumns(rowAuxColumns);
	//rowAuxColumns.dumpInfo();
	rowAuxColumns.getColumnNamesCSV(auxCol);
	rowAuxColumns.getColumnValuesCSV(auxVal, "\'");
	rowAuxColumns.getColumnsCSV(auxColVal, "\'");


	ss << "INSERT INTO "TABLE_OSS_CMTS_INTERFACE;
	ss << " (cmts_id, interface_id, interface_type, interface_desc, count_valid, interface_name";

	if (auxCol.size() > 0) {
		ss << ", " << auxCol;
	}

	ss << " ) ";
	ss << " VALUES( " << getCmtsId() << ", " << ifId << ", ";
	ss << nIfType << ", '" << ifName << "', 0, '" << ifDesc << "'";

	if (auxVal.size() > 0) {
		ss << ", " << auxVal;
	}

	ss << ") ";

	ss << " ON DUPLICATE KEY UPDATE ";
	ss << " interface_type = " << nIfType << ", ";
	ss << " interface_desc = '" << ifName << "', "; // TODO: Here name/desc are switched(UI compatibility)
	ss << " interface_name = '" << ifDesc << "', ";

	// Count invalid param set timer
	if (this->isCounterValid()) {
		ss << " count_valid = 0";
	} else {
		ss << " count_valid = count_valid + 1";
	}

	if (auxColVal.size() > 0) {
		ss << ", " << auxColVal;
	}

	//cout << "SQL: " << ss.str() << endl;

	result = ss.str();
}

/**
 * Make SQL string for INSERT single all snmp values row.
 */
void CCmtsIf::getSqlInsertSnmpRow(string& sqlInsert) {

	string tableIf = m_tools_sql.initCmtsIfTable();

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	sqlInsert = "INSERT INTO " + tableIf;

	this->prepareSqlInsertSnmpRow(sqlInsert);

	//cout << "SQL: " << sqlInsert << endl;

}

void CCmtsIf::getSqlInsertSnmpRowToday(string& result) {

	result = "INSERT DELAYED INTO ";
	result += TABLE_NAME_PREFIX_OSS_CMTS_IF;
	result += TABLE_NAME_POSTFIX_TODAY;

	this->prepareSqlInsertSnmpRow(result);

}

/**
 * Prepare common part of sql snmp row insert.
 * TODO: Remove all db related stuff in more abstract area.
 */
void CCmtsIf::prepareSqlInsertSnmpRow(string& result) {

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	stringstream sqlColumns;
	stringstream ssValues;

	time_t dumpDate = time(NULL);

	ssValues.clear();

	CSnmpValue* pSnmpValue;
	resetSnmpValueIterator();

	while (m_snmp_value_set.getNextSnmpValueFnCol(&pSnmpValue)) {

		CSnmpValue& rSnmpValue = *pSnmpValue;
		const CSnmpOid& rSnmpOid = pSnmpValue->getSnmpOid();

		// Check is Saveable OID
		if (isSaveableOid(rSnmpOid)) {
			sqlColumns << rSnmpOid.getProperty(KEY_PARAM_COL_KEY) << ", ";
			ssValues << rSnmpValue.getValue() << ", ";
		}

	}

	// Add cmts/id, date
	sqlColumns << "cmts_id, if_id, dumpdate";
	ssValues << getCmtsId() << ", " << getParam(KEY_SNMP_OID_IF_ID) << ", "
			<< dumpDate;

	result += " (" + sqlColumns.str() + ") ";
	result += " VALUES (" + ssValues.str() + ") ";

}

/**
 * Check if the OID is speed and the IF is Ethernet
 *
 */
bool CCmtsIf::isEthernetSpeed(const CSnmpOid& oid) {
	return this->isEthernet() && oid.isDimMbit();
}

/**
 * Common Check should the OID param be Saved.
 */
bool CCmtsIf::isSaveableOid(const CSnmpOid& oid) {

	bool result;

	result = this->isOidSameStream(oid); // Check if the Oid and IF are same stream
	result = result || this->isEthernetSpeed(oid);

	return result;
}

/**
 * Check IF is Ethernet
 */
bool CCmtsIf::isEthernet() {

	if (getType() == KEY_CHN_ETHERNET) {
		return true;
	} else {
		return false;
	}
}

/**
 * Check if current IF is Saveable.
 * Save streams, ethernets...
 */
bool CCmtsIf::isSaveable() {

	bool result;

	result = this->isStream();
	result = result || this->isEthernet();

	return result;
}

void CCmtsIf::setIfId(const string& if_id) {
	this->setParam(KEY_SNMP_OID_IF_ID, if_id);
}

/**
 * Get Count of Active CMs attached to this interface
 */
int CCmtsIf::getActiveCmCount() {

	string snmpVal;

	if (getSnmpValue(KEY_COL_KEY_NUM_ACTIVE_CM, snmpVal) == false) {
		snmpVal = "0";
	}

	int numActive = atoi(snmpVal.c_str());

	return numActive;
}

/**
 * Check ValidationCounter is valid.
 *
 * Just validate the Param Set for now...
 *
 */
bool CCmtsIf::isCounterValid() {

	bool result;

	result = this->isIfParamSetValid();

	return result;
}

/**
 * Check is there any in/out activity for the interface.
 *
 */
bool CCmtsIf::isInOutActive() {

	bool result = true;

	CSnmpValue* pSnmpValueIfIn = this->m_snmp_value_set.getSnmpValue(KEY_COL_KEY_IF_IN);
	CSnmpValue* pSnmpValueIfOut = this->m_snmp_value_set.getSnmpValue(KEY_COL_KEY_IF_OUT);

	if (	(!pSnmpValueIfIn || (pSnmpValueIfIn && pSnmpValueIfIn->isZero())) &&
			(!pSnmpValueIfOut || (pSnmpValueIfOut && pSnmpValueIfOut->isZero()))
	) {
		result = false;
	} else {
		result = true;
	}

	return result;
}

/**
 * Check if the docsis interface(stream) corresponding SNR is valid.
 * If other type of interface - active by default
 */
bool CCmtsIf::isSnrActive() {

	bool status = true;

	if (this->isStream()) {
		CSnmpValue* pSnmpValue = getValueSnr();

		if (!pSnmpValue || (pSnmpValue && pSnmpValue->isZero())) {
			status = false;
		} else {
			status = true;
		}
	} else {
		status = true;
	}

	return status;
}

/**
 * Do Aux Snmp List for some additional IF params from CMTS.
 *
 * TODO: Make proper snmp oids into db cpe oid types.
 */
void CCmtsIf::doCmtsIfSnmpListAux(vector<CSnmpOid>& listSnmp) {


}

/**
 * Read avg Dwn TxPwr from CM List in DB for a Upstream.
 * Write it only for Upstream
 */
void CCmtsIf::addUpDwnTxPwr(stringstream& sqlColumns,
							stringstream& sqlValues,
							stringstream& sqlUpdates,
							CMySqlDB* pDb) {

	string colKey;
	string colVal;

	stringstream sqlSelect;

	// Add dwn txpwr only for Upstreams
	if (isUpstream()) {

		sqlSelect << "SELECT sum(" << KEY_COL_DWN_TXPWR << ")/count(" << KEY_COL_DWN_TXPWR << ") AS " << KEY_COL_DWN_TXPWR;
		sqlSelect << " FROM " << TABLE_OSS_IF_LATEST << ", " << TABLE_OSS_CM_LIST;
		sqlSelect << " WHERE " << TABLE_OSS_IF_LATEST << ".cmts_id = " << TABLE_OSS_CM_LIST << ".cmts_id ";
		sqlSelect << " AND " << TABLE_OSS_IF_LATEST << ".if_id = " << TABLE_OSS_CM_LIST << ".if_dwn";
		sqlSelect << " AND " << TABLE_OSS_IF_LATEST << ".cmts_id = " << getCmtsId();
		sqlSelect << " AND " << TABLE_OSS_CM_LIST << ".if_up = " << getIfId();
		sqlSelect << " AND cm_status = " << STATUS_CM_REGISTRATION_COMPLETE;

		CDataSet ds;

		pDb->doSelect(sqlSelect.str(), ds);

		//ds.dumpInfo();

		if (ds.size() == 1) {
			CDataRow row;

			ds.getRow(0, row);

		//	row.dumpInfo();
			
			row.getColumn(KEY_COL_DWN_TXPWR, colVal);

			colKey = KEY_COL_DWN_TXPWR;
		
			if (colVal.size() > 0) {
				sqlColumns << colKey << ", ";
				sqlValues << colVal << ", ";
				sqlUpdates << colKey << " = " << colVal << ", ";
			} else {
		//		cout << __FUNCTION__ << ":"<< __LINE__ <<":pid:" << getpid();
		//		cout << " Error: row column not found SQL:[" << sqlSelect.str() << "]" << endl;
			}
		} else {
		//	cout << __FUNCTION__ << ":pid:" << getpid();
		//	cout << " Error:Sql: [" << sqlSelect.str();
		//	cout << "] returned " << ds.size() << " rows" << endl;
		}
	}

}

const string CCmtsIf::getIfId() const {
	return getParam(KEY_SNMP_OID_IF_ID);
}

/**
 * Make SQL string for INSERT/UPDATE into IF Latest function values
 */
void CCmtsIf::getSqlSaveIntoIfLatest(string& sqlInsert, CMySqlDB* pDb) {

	stringstream sqlColumns;
	stringstream ssValues;
	stringstream sqlUpdates;

	time_t dumpDate = time(NULL);

	ssValues.clear();

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	sqlInsert = "INSERT INTO "TABLE_OSS_IF_LATEST;

	CSnmpValue* pSnmpValue;
	resetSnmpValueIterator();

	while (getNextSnmpValueFnCol(&pSnmpValue)) {

		CSnmpValue& rSnmpValue = *pSnmpValue;
		const CSnmpOid& rSnmpOid = pSnmpValue->getSnmpOid();

		// Check is Saveable OID
		if (isSaveableOid(rSnmpOid)) {

			string rColKey;
			const string& rColVal = rSnmpValue.getValue();

			rSnmpOid.getProperty(KEY_PARAM_COL_KEY, rColKey);

			sqlColumns << rColKey << ", ";
			ssValues << rColVal << ", ";

			sqlUpdates << rColKey << " = " << rColVal << ", ";

		}

	}

	// Add Dwn TxPwr only for Upstreams
	addUpDwnTxPwr(sqlColumns, ssValues, sqlUpdates, pDb);

	// Add cmts/id, date
	sqlColumns << "cmts_id, if_id, dumpdate";
	ssValues << getCmtsId() << ", " << getParam(KEY_SNMP_OID_IF_ID) << ", " << dumpDate;

	sqlUpdates << "dumpdate = " << dumpDate;

	sqlInsert += " (" + sqlColumns.str() + ") ";
	sqlInsert += " VALUES (" + ssValues.str() + ") ";

	sqlInsert += " ON DUPLICATE KEY UPDATE ";
	sqlInsert += sqlUpdates.str();

	//cout << "SQL: " << sqlInsert << endl;

}

/**
 * Insert/Update into IF List - if id, type, desc, alias...
 */
bool CCmtsIf::sqlSaveIntoIfList(CMySqlDB* pDb) {

	string sqlSave;

	getSqlSaveIntoIfList(sqlSave);

	//cout << __FUNCTION__ << ":pid:" << getpid() << " cmts:" << getCmtsId() << ":if:" << getParam(KEY_SNMP_OID_IF_ID) << ": SQL: " << sqlSave << endl;

	pDb->doExecute(sqlSave);

	return true;
}

/**
 * Insert/Update into IF Latest (for all latest function params values)
 */
bool CCmtsIf::sqlSaveIntoIfLatest(CMySqlDB* pDb) {

	string sqlSave;

	getSqlSaveIntoIfLatest(sqlSave, pDb);

	//cout << __FUNCTION__ << ":pid:" << getpid() << " cmts:" << getCmtsId() << ":if:" << getParam(KEY_SNMP_OID_IF_ID) << ": SQL: " << sqlSave << endl;

	pDb->doExecute(sqlSave);

	return true;
}

/**
 * Insert into oss_cm_errors Latest IF Error Counters
 */
bool CCmtsIf::sqlSaveIntoIfErrors(CMySqlDB* pDb) {

	string sqlSave;

	getSqlSaveIntoIfErrors(sqlSave);

	//cout << __FUNCTION__ << ": SQL: " << sqlSave;

	pDb->doExecute(sqlSave);

	return true;
}

/**
 * Insert into Snmp Function Table single Row with all Snmps.
 * Insert only for up/down streams.
 */
bool CCmtsIf::sqlInsertSnmpRow(CMySqlDB* pDb) {

	if (this->isSaveable()) {

		string sqlInsert;

		getSqlInsertSnmpRow(sqlInsert);
		//cout << __FUNCTION__ << ": SQL: " << sqlInsert;
		pDb->doInsert(sqlInsert);

		getSqlInsertSnmpRowToday(sqlInsert);
		//cout << __FUNCTION__ << "SQL: " << sqlInsert << endl;
		try {
			pDb->doInsert(sqlInsert);
		} catch (CErrorSql& e) {
			cout << "ERROR: getSqlInsertSnmpRowToday: " << e.getMessage() << endl;
		}

	}

	return true;
}

/**
 * Dump CM Info (for Debug)
 */
void CCmtsIf::dumpInfo() {
	cout << "PID: " << getpid() << endl;

	this->m_row_params.dumpInfo();

	//m_snmp_value_set.dumpInfo();

	cout << endl;
}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */


