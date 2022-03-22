/*
 * CCmtsElement.h
 *
 *	CMTS Elements Base class. Should represent CMTS oriented elements.
 *	For example cmts CMs, interfaces....
 *
 *  Created on: 13.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCMTSELEMENT_H_
#define CCMTSELEMENT_H_

#include <map>
#include <vector>
#include <string>

#include "CSnmpOid.h"
#include "CCmtsToolsSql.h"
#include "SnmpOids.h"
#include "CSnmpValue.h"
#include "CSnmpValueSet.h"
#include "CSnmpValueDocs3.h"
#include "CSnmpValueDocs3Set.h"

#include <softel/db/CSqlDbFactory.h>
#include <softel/core/net/CIp.h>
#include <softel/db/CMySqlDB.h>
#include <softel/core/error/CErrorCommon.h>


namespace SF {
namespace OSS {
namespace CMTS {

class CCmtsElement {

public:

	CCmtsElement();
	~CCmtsElement();

	void setCmtsIp(const std::string&);
	void setCmtsId(const std::string& cmts_id);
	void setCmtsId(const int cmts_id);
	std::string getCmtsId() const;
	std::string getCmtsIp() const;

	void setAccountDb(SF::DB::CSqlDbFactory*);
	CCmtsToolsSql& getSqlTools();
	void setDbConn(SF::DB::CMySqlDB*);
	SF::DB::CMySqlDB* getDbConn();

	void setRCommunity(const std::string& cmts_rcommunity, const std::string& cm_rcommunity);
	void getRCommunity(std::string& cmts_rcommunity, std::string& cm_rcommunity) const;
	std::string getRCommunityCmts() const;
	std::string getRCommunityCm() const;

	void setEventMessage(const std::string& message);
	void addEventMessage(const std::string& message);

	void setParam(const std::string& key, const std::string& val);
	void setParam(const CSnmpOid& snmpOid, const std::string& val);
	void setParam(const SF::DB::CDataRow& row); // Set Params from a DataRow
	void setSnmpValue(const CSnmpOid& snmpOid, const std::string& rawSnmpValue);
	void setSnmpOidList(std::vector<CSnmpOid>* val);

	bool setDocs3SnmpValue(const CSnmpOid& oid, const std::string& if_id, const std::string& snmpValue);

	CSnmpValue* getSnmpValueDocs3(const CSnmpOid& oid, const std::string& if_id);

	std::string getParam(const std::string& key) const;
	void getParam(SF::DB::CDataRow& row) const;

	bool getSnmpValue(const std::string& key, std::string& value);
	CSnmpValue* getSnmpValue(const std::string& key);
	int getParamCount();

	void resetSnmpValueIterator();
	bool getNextSnmpValue(CSnmpValue**);
	bool getNextSnmpValueCW(CSnmpValue**);
	bool getNextSnmpValueErrorsCW(CSnmpValue**);
	bool getNextSnmpValueFnCol(CSnmpValue** pSnmpVal);

	bool calcErrorsCounters(); // Calculate Error counters as % (dUnCorrected, dCorrected)
	void setErrorCountersRow(SF::DB::CDataRow&);

protected:

	SF::DB::CDataRow m_row_params;

	std::vector<CSnmpOid> m_snmp_oid; // Full Snmp Oid list
	std::vector<CSnmpOid>* m_pSnmpOidList;

	CSnmpValueSet m_snmp_value_set;
	CSnmpValueDocs3Set m_snmp_value_set_docs3;

	SF::DB::CSqlDbFactory* m_dbFactory;
	CCmtsToolsSql m_tools_sql;
	SF::DB::CMySqlDB* m_dbConn;

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CCMTSELEMENT_H_ */
