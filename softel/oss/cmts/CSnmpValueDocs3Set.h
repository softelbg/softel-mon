/*
 * CSnmpValueDocs3Set.h
 *
 *  Created on: 16.03.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSNMPVALUEDOCS3SET_H_
#define CSNMPVALUEDOCS3SET_H_

#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include "CSnmpOid.h"
#include "SnmpOids.h"
#include "CSnmpValue.h"
#include "CSnmpValueDocs3.h"
#include <softel/db/CPosIndex.h>
#include <softel/db/CPosIndexList.h>

namespace SF {
namespace OSS {
namespace CMTS {

class CSnmpValueDocs3Set {

public:

	CSnmpValueDocs3Set();
	~CSnmpValueDocs3Set();

	void clear();
	int size() const;

	bool isParamSetValid();

	bool setDocs3SnmpValue(const CSnmpValueDocs3&);
	bool setDocs3SnmpValue(const CSnmpOid& oid, const std::string& if_id, const std::string& snmpValue);

	CSnmpValueDocs3* get(const std::string& idx, const std::string& key);
	CSnmpValueDocs3* get(const std::string& oidId);
	CSnmpValueDocs3* get(const CSnmpOid& oid);
	CSnmpValueDocs3* get(const int pos);

	CSnmpValueDocs3* getDimDb();

	void dumpInfo();
	void dumpSnmpValueListInfo(const std::string& separator, std::string& result);

private:

	std::vector<CSnmpValueDocs3>	m_set;

	SF::DB::CPosIndexList	m_idx_list;

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */


#endif /* CSNMPVALUEDOCS3SET_H_ */
