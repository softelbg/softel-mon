/*
 * CSnmpOid.h
 *
 *	Snmp Oid
 *
 *  Created on: 03.11.2011
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSnmpOid_H
#define CSnmpOid_H

#include "SnmpOids.h"
#include "SqlTableNames.h"
#include "CMySqlConn.h"
#include "BSnmpOidBase.h"

#include <softel/db/CDataSet.h>
#include <softel/db/CDataRow.h>
#include <softel/db/CMySqlDB.h>

namespace SF {
namespace OSS {
namespace CMTS {

class CSnmpOid : public BSnmpOidBase {
public:
	CSnmpOid(const SF::DB::CDataRow& row);
	CSnmpOid(int oid_id);
	CSnmpOid(const std::string& oid_id);
	CSnmpOid(const std::string& snmp_oid, int oid_id);
	CSnmpOid();
	~CSnmpOid();

	virtual void initFromDB();

	std::string getColKey() const;
	
	bool isFunction() const; 	// Is Function SNMP - function of time
	bool isValid() const; 	// Is Valid SNMP - should priority below 100
	bool isCM() const; 		// Is CM SNMP - should be CM param (from_cm [0;1])
	bool isIf() const; 		// Is IF SNMP - should be IF param (from_cm = 2)
	bool hasColumn() const;	// Has Column Key - if col_key is available should be saved into db.
	bool isAvgFromCM() const; // Is calculated avg from CMs - now snmp oid is empty for these
	bool isFnAvgFromCM() const; // Is Function and calculated avg from CMs
	bool isIfOnDemand() const; // Is IF SNMP used on demand only

	bool isUpstream() const;
	bool isUpstreamStrict() const;
	bool isDownstream() const;
	bool isDownstreamStrict() const;

	bool isDim(const std::string& dim) const; // Major is Dimension method, used by other isDim
	bool isDimCW() const;		// Is Dimension CW - these are unerrored, corrected, uncorrected...
	bool isDimdB() const;		// Is value a dB
	bool isDimdBm() const;		// Is value a dBm
	bool isDimMbit() const;		// Is value a Mbit

	static void dumpOidList(std::vector<CSnmpOid>& oidList);

private:


};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif
