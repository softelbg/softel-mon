/**
 * CMTS SQL Tools
 *
 *	@author sgeorgiev
 *
 *		www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 *
 */

#include "CCmtsToolsSql.h"

#include <softel/core/net/CIp.h>
#include <softel/core/error/CErrorSql.h>
#include <softel/core/error/CErrorCommon.h>

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::NET;

namespace SF {
namespace OSS {
namespace CMTS {

CCmtsToolsSql::CCmtsToolsSql() {

	m_sql_conn = NULL;

	setCmtsId(0);

	initOidParamsList();
}

CCmtsToolsSql::CCmtsToolsSql(CMySqlDB* pSqlConn) {
	m_sql_conn = pSqlConn;

	initOidParamsList();
}

CCmtsToolsSql::CCmtsToolsSql(const string& cmts_id) {

	setCmtsId(cmts_id);

	initOidParamsList();
}

CCmtsToolsSql::~CCmtsToolsSql() {
}

bool CCmtsToolsSql::updateTodayCmMinDate(CMySqlDB* pDb) {

	string prefix = TABLE_NAME_PREFIX_OSS_CM;

	return this->updateTodayMinDate(pDb, prefix);
}
bool CCmtsToolsSql::updateTodayIfMinDate(CMySqlDB* pDb) {

	string prefix = TABLE_NAME_PREFIX_OSS_CMTS_IF;

	return this->updateTodayMinDate(pDb, prefix);
}
bool CCmtsToolsSql::updateTodayMinDate(CMySqlDB* pDb, const string& tablePrefix) {

	int result = 0;

	stringstream sqlUpdate;

	string tableToday = tablePrefix + TABLE_NAME_POSTFIX_TODAY;

	sqlUpdate << "INSERT INTO " << TABLE_OSS_STORE;
	sqlUpdate << " (`key`, `value`) ";
	sqlUpdate << " VALUES( ";
	sqlUpdate << "\'" << tableToday << KEY_POSTFIX_MIN_DATE;
	sqlUpdate << "\', ";
	sqlUpdate << "(SELECT MIN(dumpdate) FROM " << tableToday << "))";
	sqlUpdate << " ON DUPLICATE KEY UPDATE ";
	sqlUpdate << " `value` = (SELECT MIN(dumpdate) FROM " << tableToday << ")";

	result = pDb->doInsert(sqlUpdate.str());

	return true;
}

bool CCmtsToolsSql::selectCmRCommList(CMySqlDB* pDb, CDataSet& ds) {

	stringstream sqlSelect;

	sqlSelect << "SELECT distinct(cm_rcommunity) FROM " << TABLE_OSS_CMTS_LIST;

	return pDb->doSelect(sqlSelect.str(), ds);
}

/**
 * Select CMTS IF On Demand List of SNMP Params.
 * Should include params with from_cm = 2, 3
 */
bool CCmtsToolsSql::selectCmtsIfOnDemandOidList(CMySqlDB* pDb,
		vector<CSnmpOid>& oidList) {

	string filter = " AND docsis = 2 ";
	filter += " AND from_cm IN (2, 3) ";

	return selectOidListDataSet(pDb, "", filter, oidList);
}

/**
 * Init Snmp Oid Params List from Oid Type table.
 *	TODO: If possible select * from oit types table and get the keys auto.
 */
void CCmtsToolsSql::initOidParamsList() {
	m_oid_params.clear();

	m_oid_params.push_back(KEY_PARAM_OID_SNMP);
	m_oid_params.push_back(KEY_PARAM_OID_ID);
	m_oid_params.push_back(KEY_PARAM_OID_LABEL);
	m_oid_params.push_back(KEY_PARAM_OID_DESC);
	m_oid_params.push_back(KEY_PARAM_OID_DIM);
	m_oid_params.push_back(KEY_PARAM_OID_FN);
	m_oid_params.push_back(KEY_PARAM_COL_KEY);
	m_oid_params.push_back(KEY_PARAM_OID_MULT);
	m_oid_params.push_back(KEY_PARAM_FROM_CM);

}

void CCmtsToolsSql::setCmtsId(const string& cmts_id) {
	m_cmts_id = cmts_id;
}

void CCmtsToolsSql::setCmtsId(const int cmts_id) {
	stringstream ss;
	ss << cmts_id;
	setCmtsId(ss.str());
}

string CCmtsToolsSql::getCmtsId() {
	return m_cmts_id;
}

void CCmtsToolsSql::setSqlConn(CMySqlDB* pSqlConn) {
	m_sql_conn = pSqlConn;
}

void CCmtsToolsSql::sqlGetCmtsInfo(const string& cmtsId, CDataRow& row) {

	CMySqlDB* pDb = CMySqlConn::getInstance()->getDbFactory()->newMySqlConn();

	sqlGetCmtsInfo(*pDb, cmtsId, row);

	delete pDb;
}

/**
 * Read CMTS Info from DB into DataRow
 */
void CCmtsToolsSql::sqlGetCmtsInfo(CMySqlDB& db, const string& cmtsId, CDataRow& row) {

	stringstream ssFilter;
	string filter;
	CDataSet dsCmtsList;

	ssFilter << "cmts_id = " << cmtsId;
	
	filter = ssFilter.str();

	sqlGetCmtsList(db, dsCmtsList, ssFilter.str());

	if (dsCmtsList.size() > 1) {
		stringstream ssErr;
		ssErr << "Error:" << __FUNCTION__ << ":" << __LINE__ << ": " << endl;
		ssErr << "FILTER: " << ssFilter.str() << endl;
		ssErr << "CMTS row should be single...";
		throw CErrorCommon(ssErr.str());
	} else if (dsCmtsList.size() == 0) {
		stringstream ssErr;
		ssErr << "Error:" << __FUNCTION__ << ":" << __LINE__ << ": " << endl;
		ssErr << "FILTER: " << ssFilter.str() << endl;
		ssErr << "CMTS Not Found...";
		LOGD << ssErr.str() << endl;
	}

	dsCmtsList.getRow(0, row);

}

/**
 * Select Cmts IF Info.
 */
void CCmtsToolsSql::sqlGetIfInfo(CMySqlDB& db, const string& cmtsId, const string& ifId, CDataRow& row) {

	stringstream ssSql;

	ssSql << "SELECT * " << " FROM " << TABLE_OSS_CMTS_INTERFACE;
	ssSql << " WHERE cmts_id = " << cmtsId;
	ssSql << " AND interface_id = " << ifId;

	selectSingleLine(db, ssSql.str(), row);
}

/**
 * Do a Select and return single selected Row if any.
 * If multiple or none Rows are selected error is thrown also.
 */
void CCmtsToolsSql::selectSingleLine(CMySqlDB& db, const string strSql, CDataRow& row) {

		CDataSet ds;

		db.doSelect(strSql, ds);

		if (ds.size() > 1) {
			stringstream ssErr;
			ssErr << "Error:" << __FUNCTION__ << ":" <<  __LINE__ << ": " << endl;
			ssErr << "SQL: " << strSql << endl;
			ssErr << "Row should be single...";
			throw CErrorCommon(ssErr.str());
		} else if (ds.size() == 0) {
			stringstream ssErr;
			ssErr << "Error:" << __FUNCTION__ << ":" << __LINE__ << ": " << endl;
			ssErr << "SQL: " << strSql << endl;
			ssErr << "Row Not Found...";
			LOGD << ssErr.str() << endl;
		}

		ds.getRow(0, row);
}

void CCmtsToolsSql::sqlGetCmInfo(CMySqlDB& db, const string& cmtsId, const string& macId, CDataRow& row) {

	stringstream ssSql;
	CDataSet dsCmList;

	ssSql << "SELECT * " << " FROM " << TABLE_OSS_CM_LIST;
	ssSql << " WHERE cmts_id = " << cmtsId;
	ssSql << " AND cm_mac = " << macId;

	db.doSelect(ssSql.str(), dsCmList);

	if (dsCmList.size() > 1) {
		stringstream ssErr;
		ssErr << "Error:" << __FUNCTION__ << ":" <<  __LINE__ << ": " << endl;
		ssErr << "SQL: " << ssSql.str() << endl;
		ssErr << "CM row should be single...";
		throw CErrorCommon(ssErr.str());
	} else if (dsCmList.size() == 0) {
		stringstream ssErr;
		ssErr << "Error:" << __FUNCTION__ << ":" << __LINE__ << ": " << endl;
		ssErr << "SQL: " << ssSql.str() << endl;
		ssErr << "CM Not Found...";
		LOGD << ssErr.str() << endl;
	}

	dsCmList.getRow(0, row);
}

/**
 * Get List of Active CMTS
 */
void CCmtsToolsSql::sqlGetActiveCmtsList(CMySqlDB& db, CDataSet& ds) {
	sqlGetCmtsList(db, ds, "active > 0");
}

/**
 * Get CMTS LIST - filtered
 */
void CCmtsToolsSql::sqlGetCmtsList(CMySqlDB& db, CDataSet& ds, const string& filter) {

	stringstream ssSql;

	ssSql << "SELECT * FROM " << TABLE_OSS_CMTS_LIST;

	ssSql << ((filter.size() > 0) ? " WHERE " + filter : "");

	db.doSelect(ssSql.str(), ds);
}

/**
 * Check if the Interface Id exists in table oss_cmts_interface.
 */
bool CCmtsToolsSql::checkInterfaceExists(CMySqlDB* pDb, const string& ifId) {

	int result = 0;
	string sqlStmt;

	CDataSet ds;

	sqlStmt = "SELECT interface_id ";
	sqlStmt += " FROM "TABLE_OSS_CMTS_INTERFACE" ";
	sqlStmt += " WHERE cmts_id = " + m_cmts_id;
	sqlStmt += " AND interface_id = " + ifId;

	pDb->doSelect(sqlStmt, ds);

	return (ds.size() == 0) ? false : true;
}

/**
 * Insert an Interface into OSS_Interface table.
 * Should be done only if the interface is not there already
 */
bool CCmtsToolsSql::insertInterface(CMySqlDB* pDb, const string& ifId,
		const string& ifName) {

	int result = 0;
	string sqlStmt;

	sqlStmt = "INSERT INTO "TABLE_OSS_CMTS_INTERFACE" ";
	sqlStmt += " VALUES(";
	sqlStmt += m_cmts_id + ", ";
	sqlStmt += ifId + ",'', '";
	sqlStmt += ifName + "')";

	result = pDb->doInsert(sqlStmt);

	return true;
}

/**
 * Insert Single SNMP value for IF
 */
bool CCmtsToolsSql::insertIfSingleSnmpValue(
									CMySqlDB* pDb,
									const string& ifId,
									CSnmpOid& oid,
									const string& oidValue) {
	int result;

	stringstream ssSql;

	ssSql << "INSERT INTO " << m_if_table_name;
	ssSql << " ( cmts_id, if_id, oid_id, oid_value, dumpdate ) VALUES( ";
	ssSql << m_cmts_id << ", " << ifId << ", " << oid.getOidId();
	ssSql << ", '" + oidValue << "', UNIX_TIMESTAMP(NOW()) )";

	result = pDb->doInsert(ssSql.str());

	return true;
}

/**
 * Insert Snmp Values from a List of CMTS IfId -> SnmpVal
 */
bool CCmtsToolsSql::insertSnmpValuesByIfId(CMySqlDB* pDb, CSnmpOid& oid,
		vector<string>& cmtsIfList, vector<string>& snmpValues) {

	unsigned int i;

	//LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	for (i = 0; i < cmtsIfList.size(); i++) {

		saveIfSingleSnmpValue(pDb, cmtsIfList[i], oid, snmpValues[i]);

	}

	return true;
}

/**
 * Insert Snmp Values from a List of SID->SnmpVal
 */
bool CCmtsToolsSql::insertSnmpValuesBySid(
								CMySqlDB* pDb,
								CSnmpOid& oid,
								vector<string>& cmSidList,
								vector<string>& snmpValues,
								map<string, string>& mapSidMac) {

	unsigned int i;

	//LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	for (i = 0; i < cmSidList.size(); i++) {

		string cmMac = mapSidMac[cmSidList[i]];

		if (cmMac.size() == 0) {
			continue;
		}

		saveSingleSnmpValue(pDb, cmMac, oid, snmpValues[i]);

	}

	return true;
}

/**
 * Update Interface Type
 */
bool CCmtsToolsSql::updateInterfaceType(	CMySqlDB* pDb,
											const string& ifId,
											const char* ifType) {

	int result = 0;
	string sqlStmt;

	sqlStmt = "UPDATE "TABLE_OSS_CMTS_INTERFACE" ";
	sqlStmt += " SET interface_type = '" + string(ifType) + "' ";
	sqlStmt += " WHERE cmts_id = " + m_cmts_id;
	sqlStmt += " AND interface_id = " + ifId;

	result = pDb->doUpdate(sqlStmt);

	return true;
}

/**
 * Select CMTS OIDs List.
 * Snmp Oids for DOCSIS3 CM params from CMTS.
 */
bool CCmtsToolsSql::selectCmtsDocs3OidList(CMySqlDB* pDb, vector<CSnmpOid>& oidList) {

	string filter = "AND docsis = 3";

	return selectOidListDataSet(pDb, VAL_FROM_CMTS_CM, filter, oidList);
}

/**
 * Select CMTS OIDs List.
 * Snmp Oids for CM params from CMTS.
 */
bool CCmtsToolsSql::selectCmtsOidList(CMySqlDB* pDb, vector<CSnmpOid>& oidList) {

	string filter = "AND docsis = 2";

	return selectOidListDataSet(pDb, VAL_FROM_CMTS_CM, filter, oidList);
}

/**
 * Select CMTS IFs OIDs List.
 * Snmp Oids for CMTS IFs params from CMTS.
 */
bool CCmtsToolsSql::selectCmtsIfOidList(CMySqlDB* pDb, vector<CSnmpOid>& oidList) {

	string filter = "AND docsis = 2";

	return selectOidListDataSet(pDb, VAL_FROM_CMTS_IF, filter, oidList);
}

/**
 * Select CM OIDs List.
 * Snmp Oids for CM params direct from CM.
 */
bool CCmtsToolsSql::selectCmOidList(CMySqlDB* pDb, vector<CSnmpOid>& oidList) {

	string filter = "AND docsis = 2";

	return selectOidListDataSet(pDb, VAL_FROM_CM, filter, oidList);
}

/**
 * Select CMTS OIDs List.
 * Snmp Oids for CM params from CMTS.
 * Used when make SNMP on Demand. List will be shorter than usual.
 *
 */
bool CCmtsToolsSql::selectCmtsOnDemandOidList(CMySqlDB* pDb, vector<CSnmpOid>& oidList) {

	string filter = "AND docsis = 2";

	//filter += " AND oid_dimension != '"KEY_PARAM_DIM_CW"' ";
	filter += " AND oid_dimension != '"KEY_PARAM_DIM_MBIT"' ";

	return selectOidListDataSet(pDb, VAL_FROM_CMTS_CM, filter, oidList);
}
/**
 * Select CM OIDs List.
 * Snmp Oids for CM params direct from CM.
 * Used when make SNMP on Demand. List will be shorter than usual.
 *
 */
bool CCmtsToolsSql::selectCmOnDemandOidList(CMySqlDB* pDb, vector<CSnmpOid>& oidList) {

	string filter = "AND docsis = 2";

	//filter += " AND oid_dimension != '"KEY_PARAM_DIM_CW"' ";
	filter += " AND oid_dimension != '"KEY_PARAM_DIM_MBIT"' ";

	return selectOidListDataSet(pDb, VAL_FROM_CM, filter, oidList);
}

/**
 * Select OID List using CDataSet.
 *
 */
bool CCmtsToolsSql::selectOidListDataSet(	CMySqlDB* pDb,
											const string& from,
											const string& filter,
											vector<CSnmpOid>& oidList) {

	CDataSet dsOidList;
	CDataRow row;
	stringstream ssSql;

	string colName;
	string colValue;

	ssSql << "SELECT * ";
	ssSql << " FROM " << TABLE_OSS_CMTS_OID_SETS << ", " << TABLE_OSS_CPE_OID_TYPES;
	ssSql << " WHERE cmts_id = " << m_cmts_id;
	ssSql << " AND oid_type = type_id ";

	if (from.size() > 0) {
		ssSql << " AND from_cm = " << from;
	}

	if (filter.size() > 0) {
		ssSql << " " << filter << " ";
	}

	ssSql << " ORDER BY oid_dimension ";

	//LOGD << "SQL: " << ssSql.str() << endl;
	pDb->doSelect(ssSql.str(), dsOidList);

	oidList.clear();

	dsOidList.resetRowPos();

	while(dsOidList.getNextRow(row)) {

		CSnmpOid snmpOid;

		row.resetColPos();

		while(row.getNextColumn(colName, colValue)) {
	
			snmpOid.addProperty(colName, colValue);
		}

		oidList.push_back(snmpOid);
	}

	return true;
}

bool CCmtsToolsSql::selectOidList(	CMySqlDB* pDb,
									const string& from,
									vector<CSnmpOid>& oidList) {

	return selectOidListDataSet(pDb, from, "", oidList);
}

/*
bool CCmtsToolsSql::selectCmList(CSqlDbFactory* pDbFactory, map<string, string>& mapSidMac) {
	CMySqlDB* pDb = pDbFactory->newMySqlConn();

	selectCmList(pDb, mapSidMac);

	delete pDb;

	return true;
}

bool CCmtsToolsSql::selectCmList(CMySqlDB* pDb, map<string, string>& mapSidMac) {

	char **pSqlField;
	string cmMac;
	string cmSid;

	selectCmList(pDb);

	mapSidMac.clear();

	while ((pSqlField = pDb->GetField()) != 0) {

		cmMac = *pSqlField;
		pSqlField = pDb->GetField();
		cmSid = *pSqlField;

		mapSidMac[cmSid] = cmMac;
	}

	return true;
}
*/
/**
 * Select CM List for the CMTS.
 * Result is saved in vectors
 */
/*
bool CCmtsToolsSql::selectCmList(	CSqlDbFactory* pDbFactory,
									vector<string>& cmMacList,
									vector<string>& cmSidList) {

	char **pSqlField;

	CMySqlDB* pDbConn = pDbFactory->newMySqlConn();

	selectCmList(pDbConn);

	while ((pSqlField = pDbConn->GetField()) != 0) {

		cmMacList.push_back(*pSqlField);

		pSqlField = pDbConn->GetField();
		cmSidList.push_back(*pSqlField);
	}

	delete pDbConn;
	
	return true;
}
*/
/**
 * Select CM List for the CMTS.
 * Result is retreived by caller.
 */
/*
bool CCmtsToolsSql::selectCmList(CMySqlDB* pDb) {

	int result = 0;
	string sqlStmt;

	sqlStmt = 	"SELECT cm_mac, cm_sid  " \
				" FROM "TABLE_OSS_CM_LIST \
				" WHERE cmts_id = " + m_cmts_id;

	result = pDb->DoQuery(sqlStmt.c_str());

	return true;
}
*/

/**
 * Create CM Docsis 3 function snmp params table.
 */
bool CCmtsToolsSql::createCmDocs3Table(CMySqlDB* pDb) {

	int result = 0;

	string sqlCreate;
	string tableColumns;
	string tableIndexes;
	string tableEngine;

	tableColumns = "`cmts_id` int(11) NOT NULL DEFAULT '0',";
	tableColumns += "`cm_mac` bigint(20) NOT NULL DEFAULT '0',";
	tableColumns += "`dumpdate` int(11) unsigned DEFAULT '0',";
	tableColumns += "`if_id` INT(10) UNSIGNED NULL DEFAULT '0',";

	addFunctionTableColumns(tableColumns);
	tableColumns += ", ";

	tableIndexes = "KEY `cmts_id` (`cmts_id`),";
	tableIndexes += "KEY `cm_mac` (`cm_mac`),";
	tableIndexes += "KEY `dumpdate` (`dumpdate`),";
	tableIndexes += "KEY `if_id` (`if_id`),";
	tableIndexes += "INDEX `Index combined 1` (`cmts_id`, `cm_mac`, `dumpdate`, `if_id`)";

	tableEngine = ") ENGINE=InnoDB CHARSET=utf8 COLLATE=utf8_general_ci";

	// Make the CM current function table
	sqlCreate = "CREATE TABLE IF NOT EXISTS `" + m_cm_docs3_table_name + "` (";
	sqlCreate += tableColumns;
	sqlCreate += tableIndexes;
	sqlCreate += tableEngine;

	//LOGD << "SQL: " << sqlCreate << endl;

	result = pDb->doExecute(sqlCreate);

	return true;
}

/**
 * Add Functional Table Columns
 */
void CCmtsToolsSql::addFunctionTableColumns(string& tableColumns) {

	tableColumns += "`dwn_txpwr` double DEFAULT 0,";
	tableColumns += "`dwn_rxpwr` double DEFAULT 0,";
	tableColumns += "`dwn_att` double DEFAULT 0,";
	tableColumns += "`dwn_snr` double DEFAULT 0,";
	tableColumns += "`dwn_mr` double DEFAULT 0,";
	tableColumns += "`dwn_to` double DEFAULT 0,";
	tableColumns += "`dwn_corrected` double DEFAULT 0,";
	tableColumns += "`dwn_uncorrected` double DEFAULT 0,";

	tableColumns += "`up_txpwr` double DEFAULT 0,";
	tableColumns += "`up_rxpwr` double DEFAULT 0,";
	tableColumns += "`up_att` double DEFAULT 0,";
	tableColumns += "`up_snr` double DEFAULT 0,";
	tableColumns += "`up_mr` int DEFAULT 0,";
	tableColumns += "`up_to` double DEFAULT 0,";
	tableColumns += "`up_corrected` double DEFAULT 0,";
	tableColumns += "`up_uncorrected` double DEFAULT 0,";

	tableColumns += "`ifIn` bigint DEFAULT 0,";
	tableColumns += "`ifOut` bigint DEFAULT 0";

}

/**
 * Create OSS_CM_<YYYYDD>_<CMTS_ID> table if not exists.
 * Tables has multiple columns for every needed snmp.
 */
bool CCmtsToolsSql::createCmTable(CMySqlDB* pDb) {

	int result = 0;
	string sqlCreate;
	string tableColumns;
	string tableIndexes;
	string tableEngine;

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	tableColumns = "`cmts_id` int(11) NOT NULL DEFAULT '0',";
	tableColumns += "`cm_mac` bigint(20) NOT NULL DEFAULT '0',";
	tableColumns += "`dumpdate` int(11) unsigned DEFAULT NULL,";

	addFunctionTableColumns(tableColumns);
	tableColumns += ", ";

	tableIndexes = "KEY `cmts_id` (`cmts_id`),";
	tableIndexes += "KEY `cm_mac` (`cm_mac`),";
	tableIndexes += "KEY `dumpdate` (`dumpdate`),";
	tableIndexes += "INDEX `Index combined 1` (`cmts_id`, `cm_mac`, `dumpdate`)";

	tableEngine = ") ENGINE=InnoDB CHARSET=utf8 COLLATE=utf8_general_ci";

	// Make the CM current function table
	sqlCreate = "CREATE TABLE IF NOT EXISTS `" + m_cm_table_name +  "` (";
	sqlCreate += tableColumns;
	sqlCreate += tableIndexes;
	sqlCreate += tableEngine;
	//LOGD << "SQL: " << sqlCreate << endl;
	result = pDb->doExecute(sqlCreate);

/*
	sqlCreate = "SET max_heap_table_size = 24*1024*1024*1024";
	result = pDb->DoQuery(sqlCreate.c_str());
	// Make the CM TODAY Table
	string tableToday = TABLE_NAME_PREFIX_OSS_CM
	tableToday += TABLE_NAME_POSTFIX_TODAY;
	sqlCreate = "CREATE TABLE IF NOT EXISTS `" + tableToday + "` (";
	tableEngine = ") ENGINE=MEMORY CHARSET=utf8 COLLATE=utf8_general_ci";

	tableIndexes = "KEY `cmts_id` USING BTREE (`cmts_id`),";
	tableIndexes += "KEY `cm_mac` USING BTREE (`cm_mac`),";
	tableIndexes += "KEY `dumpdate` USING BTREE (`dumpdate`),";
	tableIndexes += "INDEX `Index combined 1` USING BTREE (`cmts_id`, `cm_mac`, `dumpdate`)";

	sqlCreate += tableColumns;
	sqlCreate += tableIndexes;
	sqlCreate += tableEngine;
	//LOGD << "SQL: " << sqlCreate << endl;
	result = pDb->DoQuery(sqlCreate.c_str());
*/

	/*
	sqlCreate = "SET max_heap_table_size = 1024*1024*1024";
	result = pDb->DoQuery(sqlCreate.c_str());
	// Make the CM Latest Table with engine MEMORY
	sqlCreate = "CREATE TABLE IF NOT EXISTS `"TABLE_OSS_CM_LATEST"` (";
	tableIndexes = "PRIMARY KEY (`cmts_id`, `cm_mac`)";
	tableEngine = ") ENGINE=MEMORY CHARSET=utf8 COLLATE=utf8_general_ci";

	sqlCreate += tableColumns;
	sqlCreate += tableIndexes;
	sqlCreate += tableEngine;
	//LOGD << "SQL: " << sqlCreate << endl;
	result = pDb->DoQuery(sqlCreate.c_str());
*/

	return true;
}

bool CCmtsToolsSql::createCmTodayTable(CMySqlDB* pDb) {

	int result = 0;

	string sqlCreate;
	string tableColumns;
	string tableIndexes;
	string tableEngine;

	sqlCreate = "SET max_heap_table_size = 24*1024*1024*1024";
	result = pDb->doExecute(sqlCreate);

	tableColumns = "`cmts_id` int(11) NOT NULL DEFAULT '0',";
	tableColumns += "`cm_mac` bigint(20) NOT NULL DEFAULT '0',";
	tableColumns += "`dumpdate` int(11) unsigned DEFAULT NULL,";

	addFunctionTableColumns(tableColumns);
	tableColumns += ", ";

	string tableToday = TABLE_NAME_PREFIX_OSS_CM
	tableToday += TABLE_NAME_POSTFIX_TODAY;
	sqlCreate = "CREATE TABLE IF NOT EXISTS `" + tableToday + "` (";
	tableEngine = ") ENGINE=MEMORY CHARSET=utf8 COLLATE=utf8_general_ci";

	tableIndexes = "KEY `cmts_id` USING BTREE (`cmts_id`),";
	tableIndexes += "KEY `cm_mac` USING BTREE (`cm_mac`),";
	tableIndexes += "KEY `dumpdate` USING BTREE (`dumpdate`),";
	tableIndexes += "INDEX `Index combined 1` USING BTREE (`cmts_id`, `cm_mac`, `dumpdate`)";

	sqlCreate += tableColumns;
	sqlCreate += tableIndexes;
	sqlCreate += tableEngine;
	//LOGD << "SQL: " << sqlCreate << endl;
	result = pDb->doExecute(sqlCreate);

	return true;
}

bool CCmtsToolsSql::createCmLatestTable(CMySqlDB* pDb) {

	int result = 0;

	string sqlCreate;
	string tableColumns;
	string tableIndexes;
	string tableEngine;

	sqlCreate = "SET max_heap_table_size = 1024*1024*1024";
	result = pDb->doExecute(sqlCreate);

	tableColumns = "`cmts_id` int(11) NOT NULL DEFAULT '0',";
	tableColumns += "`cm_mac` bigint(20) NOT NULL DEFAULT '0',";
	tableColumns += "`dumpdate` int(11) unsigned DEFAULT NULL,";

	addFunctionTableColumns(tableColumns);
	tableColumns += ", ";

	// Make the CM Latest Table with engine MEMORY
	sqlCreate = "CREATE TABLE IF NOT EXISTS `"TABLE_OSS_CM_LATEST"` (";
	tableIndexes = "PRIMARY KEY (`cmts_id`, `cm_mac`)";
	tableEngine = ") ENGINE=MEMORY CHARSET=utf8 COLLATE=utf8_general_ci";

	sqlCreate += tableColumns;
	sqlCreate += tableIndexes;
	sqlCreate += tableEngine;
	//LOGD << "SQL: " << sqlCreate << endl;
	result = pDb->doExecute(sqlCreate);

	return true;
}

bool CCmtsToolsSql::createCmListDocs3Table(CMySqlDB* pDb) {

	int result = 0;

	string sqlCreate;
	string tableColumns;
	string tableIndexes;
	string tableEngine;

	sqlCreate = "SET max_heap_table_size = 1024*1024*1024";
	result = pDb->doExecute(sqlCreate);

	tableColumns = "`cmts_id` int(11) NOT NULL DEFAULT '0',";
	tableColumns += "`cm_mac` bigint(20) NOT NULL DEFAULT '0',";
	tableColumns += "`if_id` int(11) NOT NULL DEFAULT '0',";
	tableColumns += "`dumpdate` int(11) unsigned DEFAULT NULL,";

	// Make the CM Latest Table with engine MEMORY
	sqlCreate = "CREATE TABLE IF NOT EXISTS `"TABLE_OSS_CM_LIST_DOCS3"` (";

	tableIndexes = "PRIMARY KEY (`cmts_id`, `cm_mac`, `if_id`), ";
	tableIndexes += "KEY `cmts_id` (`cmts_id`), ";
	tableIndexes += "KEY `cm_mac` (`cm_mac`), ";
	tableIndexes += "KEY `if_id` (`if_id`)";

	tableEngine = ") ENGINE=MEMORY CHARSET=utf8 COLLATE=utf8_general_ci";

	sqlCreate += tableColumns;
	sqlCreate += tableIndexes;
	sqlCreate += tableEngine;
	//LOGD << "SQL: " << sqlCreate << endl;
	result = pDb->doExecute(sqlCreate);

	return true;
}

bool CCmtsToolsSql::deleteOldCmToday(CMySqlDB* pDb) {

	int period = 3600 * (24*7 + 1); // TODO: place it into config file.

	string prefix = TABLE_NAME_PREFIX_OSS_CM;

	return this->deleteOldToday(pDb, prefix, period);
}
bool CCmtsToolsSql::deleteOldIfToday(CMySqlDB* pDb) {

	int period = 3600 * (24*7 + 2); // TODO: place it into config file.

	string prefix = TABLE_NAME_PREFIX_OSS_CMTS_IF;

	return this->deleteOldToday(pDb, prefix, period);
}
/**
 * Delete Old CM Today function params (should leave only youngest param records).
 *
 */
bool CCmtsToolsSql::deleteOldToday(	CMySqlDB* pDb,
									const string& tablePrefix,
									const int period) {

	int result = 0;

	stringstream sqlSelectMinDate;
	stringstream sqlDelete;

	string tableToday = tablePrefix + TABLE_NAME_POSTFIX_TODAY;

	CDataSet ds;
	sqlSelectMinDate << "SELECT MIN(dumpdate) AS mindate, ";
	sqlSelectMinDate << " (unix_timestamp(now()) - min(dumpdate) > " << period << ") AS flagdate ";
	sqlSelectMinDate << " FROM " << tableToday;
	pDb->doSelect(sqlSelectMinDate.str(), ds);

	if (ds.size() > 0) {

		CDataRow row;

		ds.getRow(0, row);

		if (row.getColumn("flagdate").compare("1") == 0) {

			sqlDelete << "DELETE FROM " << tableToday;
//			sqlDelete << " WHERE dumpdate = " << row.getColumn("mindate");
			sqlDelete << " WHERE dumpdate < (" << row.getColumn("mindate") << " + 1800)";

			result = pDb->doExecute(sqlDelete.str());

		}

	}

	return true;
}

bool CCmtsToolsSql::deleteCmListDocs3(CMySqlDB* pDb) {

	int result = 0;

	stringstream sqlDelete;

	sqlDelete << "DELETE FROM " << TABLE_OSS_CM_LIST_DOCS3;
	sqlDelete << " WHERE cmts_id = " << m_cmts_id;

	result = pDb->doExecute(sqlDelete.str());

	return true;
}

bool CCmtsToolsSql::deleteCmCpeList(CMySqlDB* pDb) {
	int result = 0;

	stringstream sqlDelete;

	sqlDelete << "DELETE FROM " << TABLE_OSS_CM_CPE_LIST;
	sqlDelete << " WHERE cmts_id = " << m_cmts_id;

	result = pDb->doExecute(sqlDelete.str());

	return true;
}

bool CCmtsToolsSql::insertCmDocs3List(	CMySqlDB* pDb,
										const string& cm_mac,
										const string& if_id) {

	int result = 0;

	time_t dumpDate = time(NULL);

	stringstream sqlInsert;

	sqlInsert << "INSERT INTO " << TABLE_OSS_CM_LIST_DOCS3;
	sqlInsert << " (cmts_id, cm_mac, if_id, dumpdate) ";
	sqlInsert << " VALUES ( ";
	sqlInsert << m_cmts_id << ", " << cm_mac;
	sqlInsert << ", " << if_id << ", " << dumpDate;
	sqlInsert << ")";

	//LOGD << __FUNCTION__ << ":pid:" << getpid() << " SQL: " << sqlInsert.str() << endl;
 
	result = pDb->doInsert(sqlInsert.str());

	return true;
}

/**
 * Select all OIDs which are non-function, col_keyed, for CM - used for oss_cm_list columns
 */
void CCmtsToolsSql::selectCmListOids(CMySqlDB* pDb, CDataSet& ds) {

	stringstream sqlSelect;

	sqlSelect << "SELECT * ";
	sqlSelect << " FROM " << TABLE_OSS_CPE_OID_TYPES;
	sqlSelect << " WHERE is_fn = 0 ";
	sqlSelect << " AND CHAR_LENGTH(col_key) > 0 ";
	sqlSelect << " AND from_cm < 2";
	sqlSelect << " AND prio < " << THRESHOLD_PRIO_INVALID;

	pDb->doSelect(sqlSelect.str(), ds);

}

/**
 * Select all OIDs which are non-function, col_keyed, for IF - used for oss_cmts_interface columns
 */
void CCmtsToolsSql::selectIfListOids(CMySqlDB* pDb, CDataSet& ds) {
	stringstream sqlSelect;

	sqlSelect << "SELECT * ";
	sqlSelect << " FROM " << TABLE_OSS_CPE_OID_TYPES;
	sqlSelect << " WHERE is_fn = 0 ";
	sqlSelect << " AND CHAR_LENGTH(col_key) > 0 ";
	sqlSelect << " AND from_cm = 2";
	sqlSelect << " AND prio < " << THRESHOLD_PRIO_INVALID;

	pDb->doSelect(sqlSelect.str(), ds);
}

/**
 * Make additional columns from list of oids
 */
void CCmtsToolsSql::doListAuxColumns(CDataSet dsListOids, string& result) {

	string colValue;
	CDataRow row;

	dsListOids.resetRowPos();

	while (dsListOids.getNextRow(row)) {

		//row.dumpInfo();

		row.getColumn(KEY_PARAM_COL_KEY, colValue);

		result += "`" + colValue + "` VARCHAR(256) NULL DEFAULT NULL,";

	}

}

/**
 * Make Additional Columns for IF List table from IF list oids.
 */
void CCmtsToolsSql::doIfListAuxColumns(CMySqlDB* pDb, string& result) {
	string colValue;
	CDataSet dsCmListOids;
	CDataRow row;

	selectIfListOids(pDb, dsCmListOids);

	doListAuxColumns(dsCmListOids, result);
}

/**
 * Make Additional Columns for CM List table from cm list oids.
 */
void CCmtsToolsSql::doCmListAuxColumns(CMySqlDB* pDb, string& result) {

	string colValue;
	CDataSet dsCmListOids;
	CDataRow row;

	selectCmListOids(pDb, dsCmListOids);

	doListAuxColumns(dsCmListOids, result);
}

/**
 * Create CM List table. Used to save current CM attributes (ip, sid, status...)
 */
bool CCmtsToolsSql::createCmListTable(CMySqlDB* pDb) {

	int result = 0;

	string sqlCreate;
	string sqlColumns;
	string sqlIndexes;
	string sqlEngine;
	string sqlAuxColumns;

	doCmListAuxColumns(pDb, sqlAuxColumns);

	sqlCreate = "SET max_heap_table_size = 1024*1024*1024";
	result = pDb->doExecute(sqlCreate);

	sqlCreate = "CREATE TABLE IF NOT EXISTS `"TABLE_OSS_CM_LIST"` ";

	sqlColumns += "(";
	sqlColumns += "`cm_mac` BIGINT(20) NOT NULL,";
	sqlColumns += "`cmts_id` INT(11) NOT NULL DEFAULT '0',";
	sqlColumns += "`cm_ip` VARCHAR(32) NOT NULL,";
	sqlColumns += "`cm_status` INT(1) NULL DEFAULT NULL,";
	sqlColumns += "`cm_sid` INT(11) NULL DEFAULT NULL,";
	sqlColumns += "`date_list` INT(11) UNSIGNED NULL DEFAULT NULL,";
	sqlColumns += "`if_dwn` INT(10) UNSIGNED NULL DEFAULT '0',";
	sqlColumns += "`if_up` INT(10) UNSIGNED NULL DEFAULT '0',";

	sqlIndexes += "PRIMARY KEY (`cm_mac`, `cmts_id`),";
	sqlIndexes += "INDEX `Index 1` (`cm_status`),";
	sqlIndexes += "INDEX `Index 2` (`cm_ip`),";
	sqlIndexes += "INDEX `Index 3` (`cm_sid`),";
	sqlIndexes += "INDEX `Index_if_up` (`if_up`),";
	sqlIndexes += "INDEX `Index_if_down` (`if_dwn`)";
	sqlIndexes += ")";

	sqlEngine += "ENGINE=MEMORY CHARSET=utf8 COLLATE=utf8_general_ci";

	sqlCreate += sqlColumns;
	sqlCreate += sqlAuxColumns;
	sqlCreate += sqlIndexes;
	sqlCreate += sqlEngine;

	result = pDb->doExecute(sqlCreate);

	// Create CM Errors Table
	createCmErrorsTable(pDb);

	return true;
}

/**
 * Create CM CPE List.
 */
bool CCmtsToolsSql::createCmCpeListTable(CMySqlDB* pDb) {

	int result = 0;

	string sqlCreate;
	string sqlColumns;
	string sqlIndexes;
	string sqlEngine;

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__;
	LOGD << " pid:" << getpid() << endl;

	sqlCreate = "SET max_heap_table_size = 1024*1024*1024";
	result = pDb->doExecute(sqlCreate);

	sqlCreate = "CREATE TABLE IF NOT EXISTS `"TABLE_OSS_CM_CPE_LIST"` ";

	sqlColumns = "(";
	sqlColumns += "`cmts_id` INT(11) NOT NULL DEFAULT '0',";
	sqlColumns += "`cm_mac` BIGINT(20) NOT NULL,";
	sqlColumns += "`dumpdate` int(11) unsigned DEFAULT NULL,";
	sqlColumns += "`cpe_mac` VARCHAR(32) DEFAULT NULL,";
	sqlColumns += "`cpe_ip` VARCHAR(32) DEFAULT NULL,";

	sqlIndexes = "KEY `cmts_id` (`cmts_id`),";
	sqlIndexes += "KEY `cm_mac` (`cm_mac`)";
	sqlIndexes += ")";

	sqlEngine += "ENGINE=MEMORY CHARSET=utf8 COLLATE=utf8_general_ci";

	sqlCreate += sqlColumns;
	sqlCreate += sqlIndexes;
	sqlCreate += sqlEngine;

	//LOGD << __FILE__ << ":" << __FUNCTION__ << ":SQL: " << sqlCreate << endl;

	result = pDb->doExecute(sqlCreate);

	return true;
}

/**
 * Create CM Errors counters tmp table.
 * Used to store last unerrored, corrected, uncorrected CW
 */
bool CCmtsToolsSql::createCmErrorsTable(CMySqlDB* pDb) {

	int result = 0;

	string sqlCreate;
	string sqlColumns;
	string sqlIndexes;
	string sqlEngine;

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	sqlCreate = "SET max_heap_table_size = 1024*1024*1024";
	result = pDb->doExecute(sqlCreate);

	sqlCreate = "CREATE TABLE IF NOT EXISTS `"TABLE_OSS_CM_ERRORS"` ";

	sqlColumns += "(";
	sqlColumns += "`cm_mac` BIGINT(20) NOT NULL,";
	sqlColumns += "`cmts_id` INT(11) NOT NULL DEFAULT '0',";
	sqlColumns += "`dumpdate` int(11) unsigned DEFAULT NULL,";
	sqlColumns += "`d_unerrored` BIGINT(20) DEFAULT 0,";
	sqlColumns += "`dwn_corrected` BIGINT(20) DEFAULT 0,";
	sqlColumns += "`dwn_uncorrected` BIGINT(20) DEFAULT 0,";
	sqlColumns += "`u_unerrored` BIGINT(20) DEFAULT 0,";
	sqlColumns += "`up_corrected` BIGINT(20) DEFAULT 0,";
	sqlColumns += "`up_uncorrected` BIGINT(20) DEFAULT 0,";

	sqlIndexes += "PRIMARY KEY (`cm_mac`, `cmts_id`)";
	sqlIndexes += ")";

	sqlEngine += "ENGINE=MEMORY CHARSET=utf8 COLLATE=utf8_general_ci";

	sqlCreate += sqlColumns;
	sqlCreate += sqlIndexes;
	sqlCreate += sqlEngine;

	//LOGD << __FILE__ << ":" << __FUNCTION__ << ":SQL: " << sqlCreate << endl;

	result = pDb->doExecute(sqlCreate);

	return true;
}

/**
 * Create IF Errors counters tmp table.
 * Used to store last unerrored, corrected, uncorrected CW
 */
bool CCmtsToolsSql::createCmtsIfErrorsTable(CMySqlDB* pDb) {

	int result = 0;

	string sqlCreate;
	string sqlColumns;
	string sqlIndexes;
	string sqlEngine;

	sqlCreate = "SET max_heap_table_size = 1024*1024*1024";
    	result = pDb->doExecute(sqlCreate);

	sqlCreate = "CREATE TABLE IF NOT EXISTS `"TABLE_OSS_IF_ERRORS"` ";

	sqlColumns += "(";
	sqlColumns += "`cmts_id` INT(11) NOT NULL DEFAULT '0',";
	sqlColumns += "`if_id` INT(11) NOT NULL DEFAULT '0',";
	sqlColumns += "`dumpdate` int(11) unsigned DEFAULT NULL,";
	sqlColumns += "`d_unerrored` BIGINT(20) DEFAULT 0,";
	sqlColumns += "`dwn_corrected` BIGINT(20) DEFAULT 0,";
	sqlColumns += "`dwn_uncorrected` BIGINT(20) DEFAULT 0,";
	sqlColumns += "`u_unerrored` BIGINT(20) DEFAULT 0,";
	sqlColumns += "`up_corrected` BIGINT(20) DEFAULT 0,";
	sqlColumns += "`up_uncorrected` BIGINT(20) DEFAULT 0,";

	sqlIndexes += "PRIMARY KEY (`cmts_id`, `if_id`)";
	sqlIndexes += ")";

	sqlEngine += "ENGINE=MEMORY CHARSET=utf8 COLLATE=utf8_general_ci";

	sqlCreate += sqlColumns;
	sqlCreate += sqlIndexes;
	sqlCreate += sqlEngine;

	//LOGD << __FUNCTION__ << ": SQL: " << sqlCreate << endl;

	result = pDb->doExecute(sqlCreate);

	return true;
}

/**
 * Create CMTS IF List table. Contain list info for the interfaces.
 */
bool CCmtsToolsSql::createCmtsIfListTable(CMySqlDB* pDb) {

	int result = 0;

	string sqlCreate;
	string sqlColumns;
	string sqlIndexes;
	string sqlEngine;
	string sqlAuxColumns;

	doIfListAuxColumns(pDb, sqlAuxColumns);

	sqlCreate = "SET max_heap_table_size = 1024*1024*1024";
	result = pDb->doExecute(sqlCreate);


	sqlCreate = "CREATE TABLE IF NOT EXISTS `"TABLE_OSS_CMTS_INTERFACE"` ";

	sqlColumns += "(";
	sqlColumns += "`cmts_id` int(11) NOT NULL DEFAULT '0',";
	sqlColumns += "`interface_id` int(10) unsigned NOT NULL,";
	sqlColumns += "`interface_type` int(1) NOT NULL,";
	sqlColumns += "`interface_desc` varchar(128) DEFAULT NULL,";
	sqlColumns += "`interface_name` varchar(128) DEFAULT NULL,";
	sqlColumns += "`count_valid` int(11) NOT NULL DEFAULT '0',";

	sqlIndexes += "PRIMARY KEY (`cmts_id`,`interface_id`),";
	sqlIndexes += "KEY `Index_interface_type` (`interface_type`),";
	sqlIndexes += "KEY `Index_count_valid` (`count_valid`)";
	sqlIndexes += ") ";

	sqlEngine += "ENGINE=MEMORY DEFAULT CHARSET=latin1";

	sqlCreate += sqlColumns;
	sqlCreate += sqlAuxColumns;
	sqlCreate += sqlIndexes;
	sqlCreate += sqlEngine;

	result = pDb->doExecute(sqlCreate);

	return true;
}

/**
 * Create OSS_IF_<YYYYDD>_<CMTS_ID>_<OID_ID> table if not exists.
 */
bool CCmtsToolsSql::createCmtsIfTable(CMySqlDB* pDb) {

	int result = 0;
	string sqlCreate;
	string tableColumns;
	string tableIndexes;
	string tableEngine;

	sqlCreate = "CREATE TABLE IF NOT EXISTS `" + m_if_table_name +  "` (";

	tableColumns = "`cmts_id` int(11) DEFAULT NULL,";
	tableColumns += "`if_id` int(11) DEFAULT NULL,";
	tableColumns += "`dumpdate` int(11) unsigned DEFAULT NULL,";

	tableColumns += "`dwn_txpwr` double DEFAULT 0,";
	tableColumns += "`dwn_rxpwr` double DEFAULT 0,";
	tableColumns += "`dwn_att` double DEFAULT 0,";
	tableColumns += "`dwn_snr` double DEFAULT 0,";
	tableColumns += "`dwn_mr` int DEFAULT 0,";
	tableColumns += "`dwn_to` double DEFAULT 0,";
	tableColumns += "`dwn_corrected` double DEFAULT 0,";
	tableColumns += "`dwn_uncorrected` double DEFAULT 0,";

	tableColumns += "`up_txpwr` double DEFAULT 0,";
	tableColumns += "`up_rxpwr` double DEFAULT 0,";
	tableColumns += "`up_att` double DEFAULT 0,";
	tableColumns += "`up_snr` double DEFAULT 0,";
	tableColumns += "`up_mr` int DEFAULT 0,";
	tableColumns += "`up_to` double DEFAULT 0,";
	tableColumns += "`up_corrected` double DEFAULT 0,";
	tableColumns += "`up_uncorrected` double DEFAULT 0,";

	tableColumns += "`ifIn` bigint DEFAULT 0,";
	tableColumns += "`ifOut` bigint DEFAULT 0,";
	tableColumns += "`num_active_cm` bigint DEFAULT 0,";
	tableColumns += "`up_avg_util` int(11) DEFAULT '0',";

	tableIndexes = "KEY `Index 1` (`if_id`),";
	tableIndexes +=	"KEY `Index 2` (`cmts_id`),";
	tableIndexes +=	"KEY `Index 3` (`dumpdate`),";
	tableIndexes += "INDEX `Index combined 1` (`cmts_id`, `if_id`, `dumpdate`)";

	tableEngine = ") ENGINE=InnoDB CHARSET=utf8 COLLATE=utf8_general_ci";

	sqlCreate += tableColumns;
	sqlCreate += tableIndexes;
	sqlCreate += tableEngine;

	result = pDb->doExecute(sqlCreate);

	sqlCreate = "SET max_heap_table_size = 1024*1024*1024";
	result = pDb->doExecute(sqlCreate);

	// Make the IF Latest Table with engine MEMORY
	sqlCreate = "CREATE TABLE IF NOT EXISTS `"TABLE_OSS_IF_LATEST"` (";
	tableIndexes = "PRIMARY KEY (`cmts_id`, `if_id`)";
	tableEngine = ") ENGINE=MEMORY CHARSET=utf8 COLLATE=utf8_general_ci";

	sqlCreate += tableColumns;
	sqlCreate += tableIndexes;
	sqlCreate += tableEngine;
	//LOGD << "SQL: " << sqlCreate << endl;
	result = pDb->doExecute(sqlCreate);

	// Create IF Errors Table
	createCmtsIfErrorsTable(pDb);

	// Create IF TODAY Table - TODO: make it in separate method like the CM do
	sqlCreate = "SET max_heap_table_size = 8*1024*1024*1024";
	result = pDb->doExecute(sqlCreate);

	string tableToday = TABLE_NAME_PREFIX_OSS_CMTS_IF;
	tableToday += TABLE_NAME_POSTFIX_TODAY;
	sqlCreate = "CREATE TABLE IF NOT EXISTS `" + tableToday + "` (";
	tableEngine = ") ENGINE=MEMORY CHARSET=utf8 COLLATE=utf8_general_ci";

	tableIndexes = "KEY `Index 1` USING BTREE (`if_id`),";
	tableIndexes += "KEY `Index 2` USING BTREE (`cmts_id`),";
	tableIndexes += "KEY `Index 3` USING BTREE (`dumpdate`),";
	tableIndexes += "INDEX `Index combined 1` USING BTREE (`cmts_id`, `if_id`, `dumpdate`)";

	sqlCreate += tableColumns;
	sqlCreate += tableIndexes;
	sqlCreate += tableEngine;
	//LOGD << "SQL: " << sqlCreate << endl;
	result = pDb->doExecute(sqlCreate);

	// Do some aux sql...
/*
	sqlCreate = "ALTER TABLE `" + m_if_table_name +  "`";
	sqlCreate += " add column num_active_cm bigint(20) default 0;";
	LOGD << "cmts:" << m_cmts_id << " SQL: " << sqlCreate << endl;
	result = pDb->DoQuery(sqlCreate.c_str());
*/

	return true;
}

bool CCmtsToolsSql::saveSingleSnmpValue(
								const string& cmMac,
								CSnmpOid& oid,
								const string& oidValue) {

	return saveSingleSnmpValue(m_sql_conn, cmMac, oid, oidValue);
}

/**
 * Thread Safe save single snmp value.
 * Serialize threaded environment here.
 */
bool CCmtsToolsSql::saveSingleSnmpValueSafe(
								CMySqlDB* pDb,
								const string& cmMac,
								CSnmpOid& oid,
								const string& oidValue) {

	bool result = true;

	//critical section - locked by mutex
	::pthread_mutex_lock(& m_mutex);

	saveSingleSnmpValue(pDb, cmMac, oid, oidValue);

	::pthread_mutex_unlock(&m_mutex);
	//end of critical section

	return result;
}

/**
 * Save single CM snmp value.
 * Insert into additive table all is_fn (is function) Oid values.
 * Update CM Latest table for all oid values.
 */
bool CCmtsToolsSql::saveSingleSnmpValue(
								CMySqlDB* pDb,
								const string& cmMac,
								CSnmpOid& oid,
								const string& oidValue) {

	bool result = true;

	//LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;
	//LOGD << "MAC:" << cmMac << " OID:" << oid.getOidId() << " VAL:" << oidValue << endl;


	string oidValueProcessed = oidValue;

	replace(oidValueProcessed.begin(), oidValueProcessed.end(), '\'', ' ');
	replace(oidValueProcessed.begin(), oidValueProcessed.end(), '\\', ' ');

	// Check is the OID type an is Function and insert into additive table the Function sample.
	if (oid.getProperty(KEY_PARAM_OID_FN).compare(VAL_IS_FN) == 0) {
		result = insertSingleSnmpValue(pDb, cmMac, oid, oidValueProcessed);
	}

	result = result && updateCmLatestSnmpValue(pDb, cmMac, oid, oidValueProcessed);

	return result;
}

/**
 * Save single IF snmp value - insert into additive table and update into latest.
 */
bool CCmtsToolsSql::saveIfSingleSnmpValue(
								CMySqlDB* pDb,
								const string& ifId,
								CSnmpOid& oid,
								const string& oidValue) {

	bool result = true;

	// Check is the OID type an is Function and insert into additive table the Function sample.
	if (oid.getProperty(KEY_PARAM_OID_FN).compare(VAL_IS_FN) == 0) {
		result = insertIfSingleSnmpValue(pDb, ifId, oid, oidValue);
	}

	result = result && updateIfLatestSnmpValue(pDb, ifId, oid, oidValue);

	return result;
}

/**
 * Insert SNMP Value record for a CM
 */
bool CCmtsToolsSql::insertSingleSnmpValue(
								CMySqlDB* pDb,
								const string& cmMac,
								CSnmpOid& oid,
								const string& oidValue) {

	int result = 0;

	stringstream ssSql;

	ssSql << "INSERT INTO " << m_cm_table_name;
	ssSql << " ( cmts_id, cm_mac, oid_id, oid_value, dumpdate ) ";
	ssSql << " VALUES( ";
	ssSql << m_cmts_id << ", " << cmMac << ", " << oid.getOidId();
	ssSql << ", '" << oidValue << "', UNIX_TIMESTAMP(NOW()) )";

	//LOGD << "SQL: " << ssSql.str() << endl;

	result = pDb->doInsert(ssSql.str());

	return true;
}

/**
 * Select Single Snmp Oid Value from CM Latest
 */
string CCmtsToolsSql::selectCmLatestOidValue(
										CMySqlDB* pDb,
										const string& cmMac,
										CSnmpOid& oid) {

	CDataSet ds;
	string sqlResult;
	stringstream ssSql;

	ssSql << "SELECT oid_value FROM " << TABLE_OSS_CM_LATEST;
	ssSql << " WHERE cmts_id = " << m_cmts_id;
	ssSql << " AND cm_mac = " << cmMac;
	ssSql << " AND oid_id = " << oid.getOidId();

	try {

		pDb->doSelect(ssSql.str(), ds);

		if (ds.size() > 0) {
			CDataRow row;
			ds.getRow(0, row);
			row.getColumn("oid_value", sqlResult);
		}

	} catch (CErrorSql& e) {

	}

	return sqlResult;
}

/**
 * Update/Insert the SNMP value into Latest CM table
 * with complementary OID(for ex. added with 100000)
 * and value is diff from current and previous values.
 *
 */
bool CCmtsToolsSql::updateCmLatestCWErrorrs(		CMySqlDB* pDb,
										const string& cmMac,
										CSnmpOid& oid,
										const string& oidValue) {

	return true;
}

/**
 * Update/Insert the SNMP value into Latest CM table
 * with complementary OID(for ex. added with 100000)
 * and value is diff from current and previous values.
 *
 */
bool CCmtsToolsSql::updateCmLatestSnmpDiffValue(
								CMySqlDB* pDb,
								const string& cmMac,
								CSnmpOid& oid,
								const string& oidValue) {

	int result = 0;
	int newSnmpOid;

	stringstream ssSelectOldValue;
	stringstream ssSql;

	newSnmpOid = oid.getOidId() + NUM_COMPLEMENTARY_SNMP_OID_OFFSET;

	ssSelectOldValue << "SELECT CAST(oid_value AS SIGNED) FROM " << TABLE_OSS_CM_LATEST << " ol ";
	ssSelectOldValue << " WHERE cmts_id = " << m_cmts_id;
	ssSelectOldValue << " AND cm_mac = " << cmMac;
	ssSelectOldValue << " AND oid_id = " << oid.getOidId();

	//	Insert/Update Latest CM oid values
	ssSql << "INSERT INTO " << TABLE_OSS_CM_LATEST;
	ssSql << " ( cmts_id, cm_mac, oid_id, oid_value, dumpdate ) ";
	ssSql << " VALUES( ";
	ssSql << m_cmts_id << ", " << cmMac << ", " << newSnmpOid;
	ssSql << ", '" << oidValue << "', UNIX_TIMESTAMP(NOW()) )";
	ssSql << " ON DUPLICATE KEY UPDATE ";
	ssSql << " oid_value = " << oidValue << " - (";
	ssSql << ssSelectOldValue.str() << "), ";
	ssSql << " dumpdate = " << " UNIX_TIMESTAMP(NOW()) ";

	result = pDb->doInsert(ssSql.str());

	return true;
}

/**
 * Update/Insert the SNMP value into Latest CM table.
 */
bool CCmtsToolsSql::updateCmLatestSnmpValue(
								CMySqlDB* pDb,
								const string& cmMac,
								CSnmpOid& oid,
								const string& oidValue) {

	int result = 0;

	stringstream ssDiffValue;
	stringstream ssSql;

	ssSql.clear();
	ssDiffValue.clear();

	// Check OID Dimension is CW (all error FCE counters).
	// TODO: Consider optimization...
	if (oid.getProperty(KEY_PARAM_OID_DIM).compare(KEY_PARAM_DIM_CW) == 0) {
		//updateCmLatestSnmpDiffValue(pDb, cmMac, oid, oidValue);
		ssDiffValue << " oid_value_diff = '" << oidValue << "' - oid_value, ";
	}

	//	Insert/Update Latest CM oid values
	ssSql << "INSERT INTO " << TABLE_OSS_CM_LATEST;
	ssSql << " ( cmts_id, cm_mac, oid_id, oid_value, dumpdate ) ";
	ssSql << " VALUES( ";
	ssSql << m_cmts_id << ", " << cmMac << ", " << oid.getOidId();
	ssSql << ", '" << oidValue << "', UNIX_TIMESTAMP(NOW()) )";
	ssSql << " ON DUPLICATE KEY UPDATE ";
	ssSql << ssDiffValue.str();
	ssSql << " oid_value = '" << oidValue << "', ";
	ssSql << " dumpdate = " << " UNIX_TIMESTAMP(NOW()) ";

	result = pDb->doInsert(ssSql.str());

	return true;
}

/**
 * Update/Insert Latest CMTS IF SNMP values into IF latest table.
 */
bool CCmtsToolsSql::updateIfLatestSnmpValue(
							CMySqlDB* pDb,
							const string& ifId,
							CSnmpOid& oid,
							const string& oidValue) {

	int result = 0;

	stringstream ssDiffValue;
	stringstream ssSql;

	ssSql.clear();
	ssDiffValue.clear();

	// Check OID Dimension is CW (all error FCE counters).
	// TODO: Consider optimization...
	if (oid.getProperty(KEY_PARAM_OID_DIM).compare(KEY_PARAM_DIM_CW) == 0) {
		ssDiffValue << " oid_value_diff = '" << oidValue << "' - oid_value, ";
	}

	//	Insert/Update Latest IF oid values
	ssSql << "INSERT INTO " << TABLE_OSS_IF_LATEST;
	ssSql << " ( cmts_id, if_id, oid_id, oid_value, dumpdate ) ";
	ssSql << " VALUES( ";
	ssSql << m_cmts_id << ", " << ifId << ", " << oid.getOidId();
	ssSql << ", '" << oidValue << "', UNIX_TIMESTAMP(NOW()) )";
	ssSql << " ON DUPLICATE KEY UPDATE ";
	ssSql << ssDiffValue.str();
	ssSql << " oid_value = '" << oidValue << "', ";
	ssSql << " dumpdate = " << " UNIX_TIMESTAMP(NOW()) ";

	result = pDb->doInsert(ssSql.str());

	return true;

}

/**
 * Make Date Param used for Table Name.
 * The date param is YYYYDD string.
 */
void CCmtsToolsSql::doTableParamDate() {
	time_t rawtime;
	struct tm * timeinfo;
	char paramDate[64] = {0};

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	sprintf(paramDate, "%04d%02d", (timeinfo->tm_year + 1900), (timeinfo->tm_mon + 1));

	m_param_date = paramDate;
}

string& CCmtsToolsSql::initCmTable() {

	initCmTable(NULL);

	return m_cm_table_name;
}
/**
 * Initialize CM Table name, create if not exist.
 */
void CCmtsToolsSql::initCmTable(CMySqlDB* pDb = NULL) {

	doTableParamDate();

	doCmTableName(atoi(m_cmts_id.c_str()));

	doCmTableNameDocs3(0, 0);

	if (pDb != NULL) {
		createCmTable(pDb);
		createCmLatestTable(pDb);
		createCmListTable(pDb);
		createCmDocs3Table(pDb);
		createCmListDocs3Table(pDb);
		createCmCpeListTable(pDb);
		createCmTodayTable(pDb);
	}

}

string& CCmtsToolsSql::initCmtsIfTable() {

	initCmtsIfTable(NULL);

	return m_if_table_name;
}
/**
 * Initialize CMTS IF Table name, create if not exist.
 */
void CCmtsToolsSql::initCmtsIfTable(CMySqlDB* pDb) {

	doTableParamDate();

	doCmtsIfTableName(atoi(m_cmts_id.c_str()));

	if (pDb != NULL) {
		createCmtsIfTable(pDb);
		createCmtsIfListTable(pDb);
	}
}

void CCmtsToolsSql::doCmTableNameDocs3(const int cmts_id = 0, const int oid_id = 0) {

	m_cm_docs3_table_name = TABLE_NAME_PREFIX_OSS_CM_DOCS3;
	m_cm_docs3_table_name += doTableNamePostfix(cmts_id, oid_id);

}

void CCmtsToolsSql::doCmTableName() {
	doCmTableName(0, 0);
}
void CCmtsToolsSql::doCmTableName(const int cmts_id) {
	doCmTableName(cmts_id, 0);
}

/**
 * Make CM SNMP records table name - for ex. oss_cm_201112.
 * Initially name should be function of YYYYMM.
 * Can include cmts id and oid id.
 */
void CCmtsToolsSql::doCmTableName(const int cmts_id, const int oid_id) {

	m_cm_table_name = TABLE_NAME_PREFIX_OSS_CM;
	m_cm_table_name += doTableNamePostfix(cmts_id, oid_id);

}

void CCmtsToolsSql::doCmtsIfTableName(const int cmts_id) {
	doCmtsIfTableName(cmts_id, 0);
}
/**
 * Make CMTS IFs SNMP records table name - for ex. oss_if_201112.
 * Initially name should be function of YYYYMM.
 * Can include cmts id and oid id.
 */
void CCmtsToolsSql::doCmtsIfTableName(const int cmts_id, const int oid_id) {

	m_if_table_name = TABLE_NAME_PREFIX_OSS_CMTS_IF;
	m_if_table_name += doTableNamePostfix(cmts_id, oid_id);

}

string CCmtsToolsSql::doTableNamePostfix(const int cmts_id = 0, const int oid_id = 0) {

	stringstream ssPostTableName;

	if (m_param_date == "") {
		doTableParamDate();
	}

	ssPostTableName << m_param_date;

	// Add CMTS ID Param if available
	if (cmts_id > 0) {
		ssPostTableName << "_" << cmts_id;
	}

	// Add OID ID Param if available
	if (cmts_id > 0 && oid_id > 0) {
		ssPostTableName << "_" << oid_id;
	}

	return ssPostTableName.str();
}

string& CCmtsToolsSql::getCmTable() {
	return m_cm_table_name;
}

string& CCmtsToolsSql::getCmTableDocs3() {
	return m_cm_docs3_table_name;
}

string& CCmtsToolsSql::getIfTable() {
	return m_if_table_name;
}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */


