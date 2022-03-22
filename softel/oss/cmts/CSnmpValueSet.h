/*
 * CSnmpValueSet.h
 *
 *  Created on: 12.03.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSNMPVALUESET_H_
#define CSNMPVALUESET_H_

#include <string>
#include <vector>
#include <map>

#include "CSnmpOid.h"
#include "CSnmpValue.h"
#include <softel/db/CDataRow.h>

namespace SF {
namespace OSS {
namespace CMTS {

class CSnmpValueSet {

public:
	CSnmpValueSet();
	~CSnmpValueSet();

	void clear();
	int size();

	bool isParamSetValid();
	bool isParamDwnSnrValid();
	bool isParamUpSnrValid();
	bool isParamValid(const std::string& param);

	void resetSnmpValueIterator();
	void incSnmpValueIterator();

	void setSnmpValue(const CSnmpOid& snmpOid, const std::string& rawSnmpValue);

	bool getNextSnmpValue(CSnmpValue** pSnmpVal);
	bool getNextSnmpValueFnCol(CSnmpValue** pSnmpVal);
	bool getNextSnmpValueCW(CSnmpValue** pSnmpVal);
	bool getNextSnmpValueErrorsCW(CSnmpValue** pSnmpVal);

	CSnmpValue* getSnmpValue(const unsigned int pos);
	CSnmpValue* getSnmpValue(const std::string& key);

	void setErrorCountersRow(SF::DB::CDataRow& row);
	bool calcErrorsCounters();

	void dumpInfo();

private:

	std::vector<CSnmpValue>	m_snmp_value_set;

	std::map<std::string, int>	m_idx_col_key;
	std::map<std::string, int>	m_idx_type_id;
	std::map<std::string, int>	m_idx_common;

	std::map<std::string, int>::iterator	m_it_idx_col_key;
	std::map<std::string, int>::iterator	m_it_idx_type_id;
	std::map<std::string, int>::iterator	m_it_idx_common;

	int				m_current_snmp_value;

	SF::DB::CDataRow		m_rowErrorCounters;

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */


#endif /* CSNMPVALUESET_H_ */
