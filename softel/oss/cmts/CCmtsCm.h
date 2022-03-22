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

#ifndef CCmtsCm_H
#define CCmtsCm_H

#include <time.h>

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>

#include "CSnmpOid.h"
#include "CCmtsToolsSql.h"
#include "SnmpOids.h"
#include "CSnmpValue.h"
#include "CSnmpValueSpeed.h"
#include "CSnmpValueSet.h"
#include "CSnmpValueDocs3.h"
#include "CSnmpValueDocs3Set.h"
#include "CCmtsCmCpeList.h"
#include "CCpe.h"
#include "CCmtsElement.h"

#include <softel/oss/snmp/CSnmpCmIp.h>
#include <softel/core/net/CIp.h>
#include <softel/db/CMySqlDB.h>
#include <softel/core/error/CErrorCommon.h>
#include <softel/db/CDataRow.h>
#include <softel/db/CIndexedVector.h>
#include <softel/db/CSqlDbFactory.h>


namespace SF {
namespace OSS {
namespace CMTS {

class CCmtsCm : public CCmtsElement
{
public:
	CCmtsCm();

	CCmtsCm(const std::string& 	cm_mac_d,
			const int 		cmts_id);

	CCmtsCm(const std::string& 	cm_mac_d,
			const int 		cmts_id,
			const std::string& 	cmts_rcommunity,
			const std::string& 	cm_rcommunity);

	CCmtsCm(const std::string& 	cm_mac_d,
			const std::string&	cm_sid,
			const int 		cmts_id,
			const std::string& 	cmts_rcommunity,
			const std::string& 	cm_rcommunity);

	CCmtsCm(const char* 	cm_mac,
			const int 		cmts_id,
			const std::string& 	cmts_rcommunity,
			const std::string& 	cm_rcommunity);

	CCmtsCm(const SF::DB::CDataRow& row);

	~CCmtsCm();


	void initFromDb(); // Initialize CM from DB - read all needed properties from DB when MAC is available.
	void init(const SF::DB::CDataRow& row);

	// Do Single SNMP GETs - used for single CM snmp query
	void doCmtsSnmpGet(const std::vector<CSnmpOid>&, std::vector<CSnmpValue>&);
	void doCmtsSnmpGetDocs3(const std::vector<CSnmpOid>& listSnmpOids, CSnmpValueDocs3Set& setSnmpValues);
	void doCmSnmpGetDocs3(const std::vector<CSnmpOid>& listSnmpOids, CSnmpValueDocs3Set& setSnmpValues);
	void doCmSnmpGet(std::vector<CSnmpOid>&, std::vector<CSnmpValue>&);
	void doSnmpGetAll(std::string& result);
	void doSnmpGetCmSpeed(std::string& result); // Get CM if in/out speeds.
	void doSnmpGetCmStatus(std::string& result); // Get CM Status

	bool save(SF::DB::CMySqlDB* pDb); // save everything

	// TODO: Remove Snmp Values as CM Params (should onlu SnmpValues be used)
	//bool processAllSnmpValues();
	//bool processSnmpValue(std::string& snmpVal);

	bool processSnmpValuesDocs3(); // process Docsis 3 SnmpValues

// 			Setters
	void setCmIp(const std::string& ip);
	void setCmIpFromMac(); // Read IP using MAC, reading from CMTS CPE IP indexed by MAC
	void setCmValidIp(); // Validate current IP and if invalid - try to find it...

	void setCmMac(const std::string& cm_mac_d);
	void setCmMacDecimal(const std::string& cm_mac_d);

	void setCmSid(const std::string& cm_sid);

	void setCmtsIpTools(SF::CORE::NET::CIp* ip_tools);
	void setCmIpTools(SF::CORE::NET::CIp* ip_tools);
	void setCmIfUp(const std::string& ifUp);
	void setCmIfDwn(const std::string& ifDwn);

	void setCmStatus(const std::string& status);

	void initCmSnmpTools();
	void closeCmSnmpSession();

	void initCmTable();
	void initCmTable(SF::DB::CMySqlDB* pDb);

//			Getters
	std::string 				getCmMac() const;
	std::string 				getCmMacHex() const;
	std::string		 		getCmSid() const;
	std::string				getCmIp() const;

	int					getCmStatus() const;
	std::string				getCmStatusStr() const;

	std::string getCmIf(const CSnmpOid& oid) const;

	SF::CORE::NET::CIp& getCmIpTools();

	void addCpe(const CCpe& cpe);

	bool isParamValid(const std::string& param);
	bool isParamSetValid();
	bool isActive(); // Is status from CMTS a registrationComplete
	bool isActiveVisible(); // Is active status and should be visible via snmp (check if dwn snr is active)
	bool isDwnSnrValid();

	bool isValid();
	bool isValidIp();


//			Maintenance....
	void dumpCmInfo();
	void dumpCmCpeList();
	void dumpCmShortInfo();

private:

	void initCmIpTools();

	void doCmStatusActions();

	void doCmListAuxColumns(SF::DB::CDataRow& row);
	void doCmCmtsSnmpListAux(std::vector<CSnmpOid>& listSnmp);
	void doCmSnmpListAuxDocs3(std::vector<CSnmpOid>& listSnmp);

	bool saveSnmpRow(SF::DB::CMySqlDB* pDb); // save all snmp values as single row

	// Save SNMP Values - TODO: remove from here....
	bool sqlSaveIntoCmList(SF::DB::CMySqlDB* pDb);
	bool sqlSaveIntoCmListDocs3(SF::DB::CMySqlDB* pDb);
	bool sqlSaveIntoCmLatest(SF::DB::CMySqlDB* pDb);
	bool sqlSaveIntoCmErrors(SF::DB::CMySqlDB* pDb);\
	bool sqlSaveIntoCmCpeList(SF::DB::CMySqlDB* pDb);
	bool sqlInsertSnmpRow(SF::DB::CMySqlDB* pDb);
	bool sqlInsertSnmpRowDocs3(SF::DB::CMySqlDB* pDb);
	void getSqlSaveIntoCmList(std::string& result);
	void getSqlSaveIntoCmLatest(std::string& result);
	void getSqlSaveIntoCmErrors(std::string& result);
	void getSqlSaveIntoCmCpeList(std::vector<std::string>& result);
	void getSqlInsertSnmpRow(std::string& result);
	void getSqlInsertSnmpRowToday(std::string& result);
	void prepareSqlInsertSnmpRow(std::string& result);
	void getSqlInsertSnmpRowDocs3(std::vector<std::string>& result);

	std::vector<CSnmpOid> m_snmp_oid_cmts; // Snmp Oid list - query CMTS
	std::vector<CSnmpOid> m_snmp_oid_cm; // Snmp Oid list - query Cable Modem directly

	CCmtsCmCpeList		m_cpe_list;

	SF::CORE::NET::CIp*	m_Ip;
	SF::CORE::NET::CIp*	m_pCmIp;

	SF::CORE::NET::CIp		m_cmIp;

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif
