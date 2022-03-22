/*
 * CSnmpValueDocs3.cc
 *
 *  Created on: 13.03.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSnmpValueDocs3.h"

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::UTIL;

namespace SF {
namespace OSS {
namespace CMTS {

CSnmpValueDocs3::CSnmpValueDocs3() {

}

CSnmpValueDocs3::CSnmpValueDocs3(const CSnmpOid& snmpOid) :
			m_snmpOid(snmpOid) {

}

CSnmpValueDocs3::~CSnmpValueDocs3() {

}

void CSnmpValueDocs3::clear() {

	m_snmp_values.clear();
	m_if_id.clear();
	m_idx_type_id.clear();
	m_idx_if.clear();
	m_idx_if_int.clear();

}

int CSnmpValueDocs3::size()  const {
	return m_snmp_values.size();
}

void CSnmpValueDocs3::resetCurrentPos() {
	m_current_pos = 0;
}

void CSnmpValueDocs3::incCurrentPos() {
	m_current_pos++;
}

void CSnmpValueDocs3::setSnmpOid(const CSnmpOid& snmpOid) {
	m_snmpOid = snmpOid;
}

bool CSnmpValueDocs3::getIfId(int pos, string& ifId) {

	bool result = false;

	if (pos < size()) {

		ifId = m_if_id[pos];

		result = true;

	}

	return result;
}

bool CSnmpValueDocs3::getIfId(CSnmpValue* pSnmpValue, string& ifId) {

	bool result = false;
	map<string, int>::iterator it;

	if(pSnmpValue) {

		string typeId;

		pSnmpValue->getSnmpOid().getProperty(KEY_PARAM_OID_ID, typeId);

		it = m_idx_type_id.find(typeId);

		if (it != m_idx_type_id.end()) {

			ifId = m_if_id[it->second];

			result = true;
		}
	}

	return result;
}

void CSnmpValueDocs3::setSnmpValue(const string& if_id, const string& val) {

	CSnmpValue snmpValue(m_snmpOid, val);

	setSnmpValue(if_id, snmpValue);
}

void CSnmpValueDocs3::setSnmpValue(const string& if_id, CSnmpValue& val) {

	map<string, int>::iterator it;

	it = m_idx_if.find(if_id);

	// Found, so update
	if (it != m_idx_if.end()) {

		m_snmp_values[it->second] = val;

	} else {
	// Not found, so add it

		string typeId;
		int pos = size();

		val.getSnmpOid().getProperty(KEY_PARAM_OID_ID, typeId);

		m_idx_if[if_id] = pos;
		m_idx_if_int[atoi(if_id.c_str())] = pos;
		m_idx_type_id[typeId] = pos;

		m_snmp_values.push_back(val);
		m_if_id.push_back(if_id);

	}

}

CSnmpValue* CSnmpValueDocs3::getNextSnmpValue() {

	CSnmpValue* result;

	result = getSnmpValue(m_current_pos);

	incCurrentPos();

	return result;
}

CSnmpValue* CSnmpValueDocs3::getSnmpValue(int pos) {

	if (pos < size()) {
		return &(m_snmp_values[pos]);
	} else {
		return NULL;
	}

}

CSnmpValue* CSnmpValueDocs3::getSnmpValueByIfIdInt(int if_id) {

	map<int, int>::iterator it;

	it = m_idx_if_int.find(if_id);

	// Found
	if (it != m_idx_if_int.end()) {

		return &(m_snmp_values[it->second]);

	} else {
		// Not found
		return NULL;
	}

}

CSnmpValue* CSnmpValueDocs3::getSnmpValue(const string& if_id) {

	map<string, int>::iterator it;

	it = m_idx_if.find(if_id);

	// Found
	if (it != m_idx_if.end()) {

		return &(m_snmp_values[it->second]);

	} else {
		// Not found
		return NULL;
	}

}

CSnmpValue* CSnmpValueDocs3::getMinSnmpValue() {

	resetCurrentPos();

	CSnmpValue* pMinSnmpVal = NULL;
	CSnmpValue* pSnmpVal = NULL;

	float currentVal = 0;
	float minVal = 1000;

	while((pSnmpVal = getNextSnmpValue())) {

		currentVal = pSnmpVal->getValueFloat();

		if (minVal > currentVal) {

			minVal = currentVal;

			pMinSnmpVal = pSnmpVal;

		}

	}

	return pMinSnmpVal;
}

const CSnmpOid& CSnmpValueDocs3::getSnmpOid() const {
	return m_snmpOid;
}

vector<string>& CSnmpValueDocs3::getIfIdList() {
	return m_if_id;
}


/**
 * Append Dumped Snmp Value to a StringStream.
 * It can be used to display the snmp
 */
void CSnmpValueDocs3::dumpStrInfo(stringstream& result) {

	for (int i = 0; i < size(); i++) {

		string oidLabel;
		string oidDim;

		const CSnmpOid& rSnmpOid = m_snmp_values[i].getSnmpOid();

		rSnmpOid.getProperty(KEY_PARAM_OID_LABEL, oidLabel);
		rSnmpOid.getProperty(KEY_PARAM_OID_DIM, oidDim);

		result << oidLabel << ": if" << i << ":" << m_if_id[i] << " ";
		result << m_snmp_values[i].getValue() << " " << oidDim << "\n";

	}

}
/**
 * Append Dumped Snmp Value info to a string.
 * It can be used to display the snmp
 */
void CSnmpValueDocs3::dumpStrInfo(string& result) {

	stringstream ss;

	dumpStrInfo(ss);

	result += ss.str();
}

void CSnmpValueDocs3::dumpInfo() {

	cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid() << " ---------- " << endl;

	for (int i = 0; i < size(); i++) {

		cout << "SnmpValue[" << i << "][" << m_if_id[i] << "]" << endl;

		m_snmp_values[i].dumpInfo();

	}
}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

