/*
 * CSnmpValueDocs3.h
 *
 *  Created on: 13.03.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSNMPVALUEDOCS3_H_
#define CSNMPVALUEDOCS3_H_

#include <iostream>
#include <stdio.h>

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "CSnmpOid.h"
#include "SnmpOids.h"
#include "CSnmpValue.h"

namespace SF {
namespace OSS {
namespace CMTS {

class CSnmpValueDocs3 {

public:

	CSnmpValueDocs3();
	CSnmpValueDocs3(const CSnmpOid& snmpOid);

	~CSnmpValueDocs3();

	void clear();
	int size() const;

	void resetCurrentPos();
	void incCurrentPos();

	void setSnmpOid(const CSnmpOid& snmpOid);

	void setSnmpValue(const std::string& if_id, const std::string& val);
	void setSnmpValue(const std::string& if_id, CSnmpValue& val);

	CSnmpValue* getNextSnmpValue();
	CSnmpValue* getSnmpValue(int pos);
	CSnmpValue* getSnmpValueByIfIdInt(int if_id);
	CSnmpValue* getSnmpValue(const std::string& if_id);

	bool getIfId(int pos, std::string&);
	bool getIfId(CSnmpValue*, std::string&);

	std::vector<std::string>& getIfIdList();

	CSnmpValue* getMinSnmpValue();

	const CSnmpOid& getSnmpOid() const;

	void dumpStrInfo(std::stringstream& result); // Append Dump snmp value
	void dumpStrInfo(std::string& result); // Append Dump snmp value
	void dumpInfo();

private:

	std::vector <CSnmpValue> m_snmp_values;
	std::vector <std::string> m_if_id;

	unsigned int m_current_pos;

	std::map<std::string, int> m_idx_if;
	std::map<int, int> m_idx_if_int;
	std::map<std::string, int> m_idx_type_id;

	CSnmpOid m_snmpOid;

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CSNMPVALUEDOCS3_H_ */
