/*
 * CThresholdResult.h
 *
 *	Threshold Result. Should contain info about a Snmp Oid comparision using some thresholds.
 *
 *  Created on: 04.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTHRESHOLDRESULT_H_
#define CTHRESHOLDRESULT_H_

#include "CThreshold.h"

#include <softel/oss/cmts/CSnmpOid.h>
#include <softel/db/CDataRow.h>
#include <softel/db/CParamSet.h>

namespace SF {
namespace OSS {
namespace ALERT {

class CThresholdResult : public SF::DB::CParamSet {
public:
	CThresholdResult();
	CThresholdResult(CThreshold* pThreshold, SF::OSS::CMTS::CSnmpOid* pSnmpOid);
	virtual ~CThresholdResult();

	void setThreshold(CThreshold* pThreshold);
	void setSnmpOid(SF::OSS::CMTS::CSnmpOid* pSnmpOid);
	void setCmRaw(SF::DB::CDataRow& row);

	CThreshold* getThreshold();
	SF::OSS::CMTS::CSnmpOid* getSnmpOid();
	const SF::DB::CDataRow& getRow();

private:
	CThreshold*		m_pThreshold;
	SF::OSS::CMTS::CSnmpOid*		m_pSnmpOid;

	SF::DB::CDataRow		m_cm_raw;

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTHRESHOLDRESULT_H_ */
