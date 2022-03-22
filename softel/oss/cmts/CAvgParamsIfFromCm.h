/*
 * CAvgParamsIfFromCm.h
 *
 *  Created on: 03.12.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *	Copyright (c) 2012 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CAVGPARAMSIFFROMCM_H_
#define CAVGPARAMSIFFROMCM_H_

#include <vector>

#include "CSnmpOid.h"
#include "CCmtsCm.h"
#include "CCmtsIf.h"
#include <softel/db/CParamSet.h>
#include "SnmpOids.h"
#include "CCmtsCmIfList.h"

namespace SF {
namespace OSS {
namespace CMTS {

class CAvgParamsIfFromCm {
public:
	CAvgParamsIfFromCm(	const std::string& cmts_id,
						vector<CSnmpOid>& snmp_oids,
						vector<CCmtsIf*>& if_list,
						std::map<std::string, int>& map_idx_if,
						vector<CCmtsCm*>& cm_list,
						CCmtsCmIfList& cm_if_list);

	virtual ~CAvgParamsIfFromCm();

	bool collectIfParamsAvgFromCM();

	void setCmtsId(const std::string& cmts_id);
	std::string getCmtsId();

	CCmtsIf* getIfById(const std::string& key);

private:

	bool collectIfNumActiveCM(CSnmpOid& snmpOid);

	SF::DB::CParamSet m_param_set;

	vector<CSnmpOid>& m_snmp_oid_cmts_if;

	vector<CCmtsIf*>& m_if_list;
	std::map<std::string, int>& m_map_idx_if;

	vector<CCmtsCm*>& m_cm_list;

	CCmtsCmIfList& m_cm_if_list;

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CAVGPARAMSIFFROMCM_H_ */
