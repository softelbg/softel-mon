/*
 * CThresholdOidList.h
 *
 *  Created on: 02.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTHRESHOLDOIDLIST_H_
#define CTHRESHOLDOIDLIST_H_

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

#include "CThresholdOid.h"
#include "CThresholdResult.h"

#include <softel/core/intf/CListInterface.h>
#include <softel/db/CDataSet.h>
#include <softel/db/CDataRow.h>
#include <softel/oss/cmts/CSnmpOid.h>

namespace SF {
namespace OSS {
namespace ALERT {

class CThresholdOidList : public SF::CORE::INTF::CListInterface {
public:
	CThresholdOidList();
	CThresholdOidList(const SF::DB::CDataSet& ds);
	virtual ~CThresholdOidList();

	void clear();
	size_t size() const;

	void initSnmpOidList();
	const SF::OSS::CMTS::CSnmpOid* getSnmpOid(const std::string& oidId) const;

	void init(const SF::DB::CDataSet& ds);

	bool add(CThresholdOid* pThreshold);
	CThresholdOid* get(size_t pos);

	void compareCmRaw(SF::DB::CDataRow& rowCmRaw, std::map<std::string, std::vector<CThresholdResult*> >& listThResults);

	void dumpInfo() const;

protected:

	std::vector<SF::OSS::CMTS::CSnmpOid>			m_snmp_oid_list;
	std::map<std::string, int>			m_idx_oid_id_snmp_oid_list;

	std::vector<CThresholdOid*>		m_pListThresholds;
	std::map<std::string, int> 			m_idx_oid_id;
	std::map<std::string, int> 			m_idx_col_key;

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTHRESHOLDOIDLIST_H_ */
