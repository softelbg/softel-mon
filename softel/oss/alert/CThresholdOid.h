/*
 * CThresholdOid.h
 *
 *	Snmp Oid based Threshold list.
 *	Should contain multiple Thresholds defined for a oid param.
 *
 *  Created on: 02.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTHRESHOLDOID_H_
#define CTHRESHOLDOID_H_

#include "CThreshold.h"
#include "CThresholdListBase.h"
#include "CThresholdResult.h"

#include <softel/oss/cmts/CSnmpOid.h>
#include <softel/oss/cmts/SnmpOids.h>
#include <softel/oss/cmts/CSnmpValue.h>

namespace SF {
namespace OSS {
namespace ALERT {

class CThresholdOid : public CThresholdListBase {
public:
	CThresholdOid(const SF::OSS::CMTS::CSnmpOid& oid);
	virtual ~CThresholdOid();

	void init(); // init from db
	void init(const SF::DB::CDataSet& ds);

	void compare(const SF::OSS::CMTS::CSnmpValue& value, CThresholdResult& result);
	void compare(const std::string& value, CThresholdResult& result);

	std::string getOidId() const;
	std::string getColKey() const;

	void dumpInfo() const;

private:

	SF::OSS::CMTS::CSnmpOid	m_snmp_oid;

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTHRESHOLDOID_H_ */
