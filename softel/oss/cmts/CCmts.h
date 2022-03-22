/*
 * CCmts.h
 *
 *	Main CMTS handler.
 *
 *	TODO: Should Refactor and Rework...
 *
 *  Created on: 01.11.2011
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCmts_H
#define CCmts_H

#include <stdio.h>
#include <time.h>
#include <sys/wait.h>

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>

#include "SnmpOids.h"
#include "CCmtsCm.h"
#include "CCmtsIf.h"
#include "CSnmpOid.h"
#include "CCmtsToolsSql.h"
#include "CLaunchSnmpFromCm.h"
#include "CSnmpToolsAsynch.h"
#include "CCmtsModProfilesList.h"
#include "CCmtsElement.h"
#include "CCmtsCmIfList.h"
#include "CAvgParamsIfFromCm.h"
#include "CCmtsLog.h"

#include <softel/db/CSqlDbFactory.h>
#include <softel/db/CSqlConnPool.h>
#include <softel/core/net/CIp.h>
#include <softel/core/util/CThreads.h>
#include <softel/core/util/CSharedCounter.h>
#include <softel/db/CDataSet.h>
#include <softel/core/util/CTimeCounter.h>
#include <softel/db/CParamSet.h>
#include <softel/core/system/CSystemTools.h>


namespace SF {
namespace OSS {
namespace CMTS {

class CCmts
{
public:
	CCmts(	const int cmts_id,
			const std::string& cmts_ip,
			const std::string& cmts_rcommunity,
			const std::string& cm_rcommunity,
			const std::string& cmts_desc,
			const int cmts_docsis_ver
		);

	CCmts(	const std::string& cmts_id,
			const std::string& cmts_ip,
			const std::string& cmts_rcommunity,
			const std::string& cm_rcommunity,
			const std::string& cmts_desc,
			const int cmts_docsis_ver
		);

	~CCmts();

	void startCmtsCollectingSingle();
	void startCmtsCollecting();

	void stopCollecting();
	bool isStoppedCollecting();

// New Walk Bulk methods - should replace old ones using snmp walk
// A MAP with all CMs is first created and all needed SNMPs are done to fill the CM
// The DB Inserts are done at last.
	bool refreshCmListBulk();
	bool updateCmParamBulk();

	bool collectSnmpCmBulkParams(); // Fill CMs Params from CMTS
	bool collectSnmpCmBulkParamsDocs3(); // Fill CMs DOCSIS 3.0 Params from CMTS
	bool collectSnmpCmBulkParamsAux(); // Fill Aux CMs Params from CMTS (CPE List...)

	bool collectSnmpFromCmAsynch();

	bool refreshInterfaces();
	bool collectCmtsIfSnmpBulk();

	bool refreshIfListBulk();
	bool updateIfParamBulk();
	bool collectSnmpIfBulkParams();
	bool collectIfParamsAvgFromCM(); // Calculate some params as averaged from CMs for this IF
	bool collectIfNumActiveCM(CSnmpOid& snmpOid); // make count of CMs attached to the interface

	size_t getCmCount();

	void setCmtsId(int cmts_id);
	void setCmtsId(const std::string& cmts_id);
	void setCmtsIp(const std::string& cmts_ip);
	void setCmtsRComunity(const std::string& cmts_rcommunity);
	void setCmRComunity(const std::string& cm_rcommunity);
	void setCmtsVer(int cmts_docsis_ver);
	void setCmtsDesc(const std::string& cmts_desc);

	void setAccountDb(SF::DB::CSqlDbFactory*);
	void setSqlConnPool(SF::DB::CSqlConnPool*);
	void setSharedCounter(SF::CORE::UTIL::CSharedCounter*);
	void setCmtsPosition(const int pos);
	void setPeriodSleep(int period);

	std::string getCmtsId();

	void dumpCmtsInfo();
	void dumpCmList();
	void dumpIfList();

private:

	bool collectCmParams(); // Should read full CM list, and all needed params(via snmp)
	bool collectIfParams(); // Should read full IF list, and all needed params(via snmp)

	bool postProcessCmList(); // process collected snmp values for all CMs
	bool postProcessIfList(); // process collected snmp values for all IFs

	bool doSnmpList();
	bool doCmtsCmSnmpList();
	bool doCmtsCmSnmpDocs3List();
	bool doCmtsIfSnmpList();
	bool doCmSnmpList();

	bool doDeleteOldCmToday();
	bool doDeleteOldIfToday();
	bool doUpdateTodayCmMinDate();
	bool doUpdateTodayIfMinDate();

	bool doCmErrorsList();
	bool doIfErrorsList();

	bool selectCmRCommList(); // Select Full CM ReadCommunity List. Used for wrong rcomm modems

	void 	initCmParamsMap();
	void	initCmTable();
	void	initCmtsIfTable();
	void 	initIfParamsMap();

	void	initModProfiles();

	// Vector CMs List handling functions. Use Map with indexes to search it
	// TODO: Start using CCmtsCmList instead...
	void clearCmList();
	void addCmBySid(const std::string& sid, CCmtsCm* cm);
	CCmtsCm* getCmBySid(const std::string& key);
	CCmtsCm* getCmByMac(const std::string& key);
	bool setCmParamBySid(const std::string& sid, const std::string& key, const std::string& val);

	void clearIfList();
	void addIfById(const std::string& if_id);
	void addIfById(const std::string& if_id, CCmtsIf* cmts_if);
	CCmtsIf* getIfById(const std::string& key);
	bool setIfParamById(const std::string& if_id, const std::string& key, const std::string& val);

	void setCmErrorsRow(CCmtsCm* cm);
	void setIfErrorsRow(CCmtsIf* pIf);

	int 	m_cmts_id;
	std::string 	m_cmts_ip;
	std::string 	m_cmts_rcommunity;
	std::string 	m_cm_rcommunity;
	std::string 	m_cmts_desc;
	int 	m_cmts_docsis_ver;

	int		m_cmts_position;

	int 	m_periodSleep;
	bool	m_stopCollecting;

	SF::DB::CParamSet m_param_set;

	SF::CORE::UTIL::CSharedCounter*	m_pSharedCounter;

	SF::DB::CSqlDbFactory* 	m_dbFactory;
	SF::DB::CSqlConnPool*	m_pDbConnPool;

	// CM List
	std::vector<CCmtsCm*> m_cm_list; // List with CM (use map indexes for search)
	std::map<std::string, int> m_map_idx_sid; // map with SID index for cm list
	std::map<std::string, int> m_map_idx_mac; // map with MAC index for cm list
	std::map<std::string, int>::iterator m_it_idx;

	// IF List
	std::vector<CCmtsIf*> m_if_list; // List with IFs (use map indexes for search)
	std::map<std::string, int> m_map_idx_if; // map with index for iFs
	std::map<std::string, int>::iterator m_it_idx_if;

	// TODO: Make CSnmpOidSet
	std::vector<CSnmpOid> m_snmp_oid; // Full Snmp Oid list
	std::vector<CSnmpOid> m_snmp_oid_cmts; // Snmp Oid list - query CMTS
	std::vector<CSnmpOid> m_snmp_oid_cmts_if; // Snmp Oid list - query CMTS for CMTS iFs
	std::vector<CSnmpOid> m_snmp_oid_cm; // Snmp Oid list - query Cable Modem directly
	std::vector<CSnmpOid> m_snmp_oid_cmts_docs3; // Snmp Oid list - query CMTS for DOCS3 CMs

	std::map<std::string, std::string> m_map_oid_cm_params; // List of SNMP OIDs for cm list table (ip, status, ifs...)
	std::map<std::string, std::string> m_map_oid_if_params; // List of SNMP OIDs for if list table (if desc, alias...)

	SF::CORE::NET::CIp* m_Ip;

	CCmtsToolsSql	m_tools_sql;

	SF::DB::CDataSet m_ds_cm_errors;
	SF::DB::CDataSet m_ds_if_errors;

	SF::DB::CDataSet m_list_cm_rcomm;

	CCmtsModProfilesList m_modprofiles;

	CCmtsCmIfList m_cm_if_list; // List of CM<->IF - when cm is on multiple interfaces.

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif
