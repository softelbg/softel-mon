/*
 * CmtsIf.h
 *
 *  Created on: 13.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCMTSIF_H_
#define CCMTSIF_H_

#include <time.h>

#include <map>
#include <vector>
#include <string>

#include "CSnmpOid.h"
#include "CCmtsToolsSql.h"
#include "SnmpOids.h"
#include "CSnmpValue.h"
#include "CSnmpValueSet.h"
#include "CCmtsModProfilesList.h"
#include "CCmtsElement.h"

#include <softel/db/CSqlDbFactory.h>
#include <softel/core/net/CIp.h>
#include <softel/db/CMySqlDB.h>
#include <softel/core/error/CErrorCommon.h>

namespace SF {
namespace OSS {
namespace CMTS {

class CCmtsIf: public CCmtsElement {

public:

	CCmtsIf();
	CCmtsIf(const int cmts_id);
	CCmtsIf(const std::string& cmts_id);
	CCmtsIf(const std::string& cmts_id, const std::string& if_id);
	~CCmtsIf();

	void initFromDb(); // Initialize CM from DB - read all needed properties from DB when MAC is available.

	// Do Single SNMP GETs - used for single IF snmp query
	void doCmtsSnmpGet(const std::vector<CSnmpOid>&, std::vector<CSnmpValue>&);
	void doSnmpGetAll(std::string& result);
	void doSnmpGetCustomOid(const std::string& oidId, std::string& result);

	bool save(SF::DB::CMySqlDB* pDb); // save everything

	const std::string getIfId() const;

	void setModProfilesList(CCmtsModProfilesList* pMp);

	void setIfId(const std::string& if_id);
	void setTypeUpstream();
	void setTypeDownstream();
	void setType(int type);
	int getType();

	CSnmpValue* getValueSnr(); // Get SNR snmp value for the interface

	bool isStream();
	bool isUpstream();
	bool isDownstream();
	bool isEthernet();
	bool isOidSameStream(const CSnmpOid& oid);
	bool isOidSameStreamStrict(const CSnmpOid& oid);
	bool isIfParamSetValid();
	bool isEthernetSpeed(const CSnmpOid& oid);
	bool isSaveableOid(const CSnmpOid& oid); // Common check should the OID value be saved
	bool isSaveable();	// Check if the IF is saveable.
	bool isCounterValid(); // Check validation counter is valid
	bool isInOutActive(); // Check is there any activity on ifIn/ifOut params
	bool isSnrActive(); // Check if corresponding SNR is valid for docsis interfaces

	int getActiveCmCount();
	void incActiveCM(CSnmpOid&);

	void dumpInfo();

private:

	void doCmtsIfSnmpListAux(std::vector<CSnmpOid>& listSnmp);

	bool sqlSaveIntoIfList(SF::DB::CMySqlDB* pDb);
	bool sqlSaveIntoIfLatest(SF::DB::CMySqlDB* pDb);
	bool sqlSaveIntoIfErrors(SF::DB::CMySqlDB* pDb);
	bool sqlInsertSnmpRow(SF::DB::CMySqlDB* pDb);
	void getSqlSaveIntoIfList(std::string&);
	void getSqlSaveIntoIfLatest(std::string& result, SF::DB::CMySqlDB* pDb);
	void getSqlSaveIntoIfErrors(std::string& result);
	void getSqlInsertSnmpRow(std::string&);
	void getSqlInsertSnmpRowToday(std::string& result);
	void prepareSqlInsertSnmpRow(std::string& result);

	void doIfListAuxColumns(SF::DB::CDataRow& row);
	bool saveSnmpRow(SF::DB::CMySqlDB* pDb); // save all snmp values as single row
	void addUpDwnTxPwr(std::stringstream&, std::stringstream&, std::stringstream&, SF::DB::CMySqlDB* pDb);

	int m_type;

	std::vector<CSnmpOid> m_snmp_oid_cmts_if; // Snmp Oid list - query CMTS

	CCmtsModProfilesList* m_pModProfiles; // Modulation Profiles List (used for modulation info)

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CCMTSIF_H_ */
