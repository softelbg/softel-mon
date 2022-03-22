/**
 * CMTS SQL Tools
 *
 *	@author sgeorgiev
 *
 *		www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCmtsToolsSql_H
#define CCmtsToolsSql_H

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <unistd.h>

#include "SqlTableNames.h"
#include "CSnmpOid.h"
#include "SnmpOids.h"
#include "CMySqlConn.h"
#include "CCmtsLog.h"

#include <softel/db/CMySqlDB.h>
#include <softel/db/CSqlDbFactory.h>
#include <softel/core/util/CThreads.h>
#include <softel/core/error/CErrorCommon.h>

namespace SF {
namespace OSS {
namespace CMTS {

class CCmtsToolsSql
{
public:
	CCmtsToolsSql();
	CCmtsToolsSql(SF::DB::CMySqlDB*);
	CCmtsToolsSql(const std::string& cmts_id);
	~CCmtsToolsSql();

	void sqlGetCmtsList(SF::DB::CMySqlDB& db, SF::DB::CDataSet& ds, const std::string& filter);
	void sqlGetActiveCmtsList(SF::DB::CMySqlDB& db, SF::DB::CDataSet& ds);

	void sqlGetCmtsInfo(const std::string&, SF::DB::CDataRow&);
	void sqlGetCmtsInfo(SF::DB::CMySqlDB& db, const std::string&, SF::DB::CDataRow&);
	void sqlGetCmInfo(SF::DB::CMySqlDB& db, const std::string&, const std::string& macId, SF::DB::CDataRow&);
	void sqlGetIfInfo(SF::DB::CMySqlDB& db, const std::string&, const std::string& ifId, SF::DB::CDataRow&);

// Selects

	void selectSingleLine(SF::DB::CMySqlDB& db, const std::string strSql, SF::DB::CDataRow& row);

	bool selectCmtsOidList(SF::DB::CMySqlDB* pDb, std::vector<CSnmpOid>& oidList);
	bool selectCmtsDocs3OidList(SF::DB::CMySqlDB* pDb, std::vector<CSnmpOid>& oidList);
	bool selectCmOidList(SF::DB::CMySqlDB* pDb, std::vector<CSnmpOid>& oidList);
	bool selectCmtsIfOidList(SF::DB::CMySqlDB* pDb, std::vector<CSnmpOid>& oidList);
	bool selectCmtsIfOnDemandOidList(SF::DB::CMySqlDB* pDb, std::vector<CSnmpOid>& oidList);
	bool selectCmtsOnDemandOidList(SF::DB::CMySqlDB* pDb, std::vector<CSnmpOid>& oidList);
	bool selectCmOnDemandOidList(SF::DB::CMySqlDB* pDb, std::vector<CSnmpOid>& oidList);

	bool selectOidListDataSet(SF::DB::CMySqlDB* pDb, const std::string& from, const std::string& filter, std::vector<CSnmpOid>& oidList);
	bool selectOidList(SF::DB::CMySqlDB* pDb, const std::string& from, std::vector<CSnmpOid>& oidList);

	/*
	bool selectCmList(SF::DB::CSqlDbFactory* pDbFactory, std::map<std::string, std::string>& mapSidMac);
	bool selectCmList(SF::DB::CMySqlDB* pDb, <std::string, std::string>& mapSidMac);
	bool selectCmList(	SF::DB::CSqlDbFactory* pDbFactory,
						std::vector<std::string>& cmMacList,
						std::vector<std::string>& cmSidList);
	bool selectCmList(SF::DB::CMySqlDB* pDb);
*/

	std::string selectCmLatestOidValue(SF::DB::CMySqlDB* pDb, const std::string& cmMac, CSnmpOid& oid);

	void selectCmListOids(SF::DB::CMySqlDB* pDb, SF::DB::CDataSet& ds);
	void selectIfListOids(SF::DB::CMySqlDB* pDb, SF::DB::CDataSet& ds);

	bool selectCmRCommList(SF::DB::CMySqlDB* pDb, SF::DB::CDataSet& ds);

// Checks
	bool checkInterfaceExists(SF::DB::CMySqlDB* pDb, const std::string& ifId);


// Save(insert/update)
	bool saveSingleSnmpValue(	const std::string& cmMac,
								CSnmpOid& oid,
								const std::string& oidValue);
	bool saveSingleSnmpValue(	SF::DB::CMySqlDB* pDb,
								const std::string& cmMac,
								CSnmpOid& oid,
								const std::string& oidValue);

	bool saveSingleSnmpValueSafe(SF::DB::CMySqlDB* pDb,
								const std::string& cmMac,
								CSnmpOid& oid,
								const std::string& oidValue);

	bool saveIfSingleSnmpValue(	SF::DB::CMySqlDB* pDb,
								const std::string& ifId,
								CSnmpOid& oid,
								const std::string& oidValue);

// Inserts
	bool insertInterface(SF::DB::CMySqlDB* pDb, const std::string& ifId, const std::string& ifName);

	bool insertSingleSnmpValue(	SF::DB::CMySqlDB* pDb,
								const std::string& cmMac,
								CSnmpOid& oid,
								const std::string& oidValue);

	bool insertSnmpValuesBySid(	SF::DB::CMySqlDB* pDb,
								CSnmpOid& oid,
								std::vector<std::string>& cmSidList,
								std::vector<std::string>& snmpValues,
								std::map<std::string, std::string>& mapSidMac);

	bool insertSnmpValuesByIfId(	SF::DB::CMySqlDB* pDb,
									CSnmpOid& oid,
									std::vector<std::string>& cmtsIfList,
									std::vector<std::string>& snmpValues);

	bool insertIfSingleSnmpValue(	SF::DB::CMySqlDB* pDb,
									const std::string& ifId,
									CSnmpOid& oid,
									const std::string& oidValue);

	bool insertCmDocs3List(SF::DB::CMySqlDB* pDb, const std::string& cm_mac, const std::string& if_id);

// Updates
	bool updateInterfaceType(SF::DB::CMySqlDB* pDb, const std::string& ifId, const char* ifType);

	bool updateCmLatestSnmpValue(	SF::DB::CMySqlDB* pDb,
									const std::string& cmMac,
									CSnmpOid& oid,
									const std::string& oidValue);

	bool updateIfLatestSnmpValue(	SF::DB::CMySqlDB* pDb,
									const std::string& ifId,
									CSnmpOid& oid,
									const std::string& oidValue);

	bool updateCmLatestSnmpDiffValue(	SF::DB::CMySqlDB* pDb,
										const std::string& cmMac,
										CSnmpOid& oid,
										const std::string& oidValue);

	bool updateCmLatestCWErrorrs(		SF::DB::CMySqlDB* pDb,
										const std::string& cmMac,
										CSnmpOid& oid,
										const std::string& oidValue);

	bool updateTodayMinDate(SF::DB::CMySqlDB* pDb, const std::string& tablePrefix);
	bool updateTodayCmMinDate(SF::DB::CMySqlDB* pDb);
	bool updateTodayIfMinDate(SF::DB::CMySqlDB* pDb);

	bool deleteCmListDocs3(SF::DB::CMySqlDB* pDb);
	bool deleteCmCpeList(SF::DB::CMySqlDB* pDb);
	bool deleteOldToday(SF::DB::CMySqlDB* pDb, const std::string& tablePrefix, const int period);
	bool deleteOldCmToday(SF::DB::CMySqlDB* pDb);
	bool deleteOldIfToday(SF::DB::CMySqlDB* pDb);

// setters
	void setCmtsId(const std::string& cmts_id);
	void setCmtsId(const int cmts_id);
	void setSqlConn(SF::DB::CMySqlDB*);

	std::string& initCmTable();
	void initCmTable(SF::DB::CMySqlDB* pDb);
	std::string& initCmtsIfTable();
	void initCmtsIfTable(SF::DB::CMySqlDB* pDb);

// getters
	std::string	getCmtsId();
	std::string& getCmTable();
	std::string& getCmTableDocs3();
	std::string& getIfTable();


private:

	void addFunctionTableColumns(std::string&);
	// Generate CM SNMP values Table Name and create if missing.
	bool createCmTable(SF::DB::CMySqlDB* pDb);
	bool createCmDocs3Table(SF::DB::CMySqlDB* pDb);
	bool createCmListTable(SF::DB::CMySqlDB* pDb);
	bool createCmListDocs3Table(SF::DB::CMySqlDB* pDb);
	bool createCmLatestTable(SF::DB::CMySqlDB* pDb);
	bool createCmErrorsTable(SF::DB::CMySqlDB* pDb);
	bool createCmCpeListTable(SF::DB::CMySqlDB* pDb);
	bool createCmTodayTable(SF::DB::CMySqlDB* pDb);

	bool createCmtsIfTable(SF::DB::CMySqlDB* pDb);
	bool createCmtsIfListTable(SF::DB::CMySqlDB* pDb);
	bool createCmtsIfErrorsTable(SF::DB::CMySqlDB* pDb);

	void doTableParamDate();
	void doCmTableName();
	void doCmTableName(const int cmts_id);
	void doCmTableName(const int cmts_id, const int oid_id);
	void doCmTableNameDocs3(const int cmts_id, const int oid_id);

	void doCmtsIfTableName(const int cmts_id, const int oid_id);
	void doCmtsIfTableName(const int cmts_id);

	std::string doTableNamePostfix(const int cmts_id, const int oid_id);

	void doListAuxColumns(SF::DB::CDataSet dsListOids, std::string& result);
	void doIfListAuxColumns(SF::DB::CMySqlDB* pDb, std::string& result);
	void doCmListAuxColumns(SF::DB::CMySqlDB* pDb, std::string& result);

	void initOidParamsList();

	SF::DB::CMySqlDB* 	m_sql_conn;

	std::string		m_cmts_id;
	//std::string		m_cmts_id_str;

	std::string		m_sql_stmt;

	std::string 		m_cm_table_name;
	std::string 		m_cm_docs3_table_name;
	std::string 		m_if_table_name;

	std::string 		m_param_date;

	std::vector<std::string>	m_oid_params;

	pthread_mutex_t m_mutex;

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif
