/*
 * CSnmpValueDocs3Set.cc
 *
 *  Created on: 16.03.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSnmpValueDocs3Set.h"

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::UTIL;

namespace SF {
namespace OSS {
namespace CMTS {

CSnmpValueDocs3Set::CSnmpValueDocs3Set() {

	clear();

}

CSnmpValueDocs3Set::~CSnmpValueDocs3Set() {

}

void CSnmpValueDocs3Set::clear() {
	m_set.clear();
	m_idx_list.clear();
}

int CSnmpValueDocs3Set::size() const {
	return m_set.size();
}

bool CSnmpValueDocs3Set::isParamSetValid() {

	return true;
}

bool CSnmpValueDocs3Set::setDocs3SnmpValue(const CSnmpValueDocs3& snmpValue) {

	// TODO: Make some automated Indexed key list...
	m_idx_list.setIdx(KEY_PARAM_OID_ID, snmpValue.getSnmpOid().getOidIdStr(), size());
	m_idx_list.setIdx(KEY_PARAM_COL_KEY, snmpValue.getSnmpOid().getColKey(), size());

	m_set.push_back(snmpValue);

	return true;
}

bool CSnmpValueDocs3Set::setDocs3SnmpValue(const CSnmpOid& oid, const string& if_id, const string& snmpValue) {

	//cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid();
	//cout << " oid:" << oid.getSnmpOid() << " if_id:" << if_id << " snmpValue:" << snmpValue << endl;

	CSnmpValueDocs3* pSnmpVal;

	pSnmpVal = get(oid);

	// Already existing
	if (pSnmpVal != NULL) {

		pSnmpVal->setSnmpValue(if_id, snmpValue);

	} else {
	// New docs3 snmp val

		CSnmpValueDocs3 snmpVal(oid);

		snmpVal.setSnmpValue(if_id, snmpValue);

		m_idx_list.setIdx(KEY_PARAM_OID_ID, oid.getOidIdStr(), size());
		m_idx_list.setIdx(KEY_PARAM_COL_KEY, oid.getColKey(), size());

		m_set.push_back(snmpVal);
	}


	return true;
}

/**
 * Get SnmpValueDocs3 by Position.
 */
CSnmpValueDocs3* CSnmpValueDocs3Set::get(const int pos) {

	CSnmpValueDocs3* pSnmpValue = NULL;

	if (pos >= 0 && pos < size()) {

		pSnmpValue = &(m_set[pos]);

	} else {

		pSnmpValue = NULL;

	}

	return pSnmpValue;
}

CSnmpValueDocs3* CSnmpValueDocs3Set::get(const string& oidId) {
	return this->get(KEY_PARAM_OID_ID, oidId);
}

/**
 * Get snmp value by SnmpOid.
 * Some chain of different indexes should be checked.
 * For example type_id, col_key...
 * TODO: As mentioned in add/set indexes, some list of keys could be used here...
 */
CSnmpValueDocs3* CSnmpValueDocs3Set::get(const CSnmpOid& oid) {

	CSnmpValueDocs3* pSnmpValue = NULL;

	pSnmpValue = this->get(oid.getProperty(KEY_PARAM_OID_ID));

	if (pSnmpValue == NULL) {
		pSnmpValue = this->get(KEY_PARAM_COL_KEY, oid.getProperty(KEY_PARAM_COL_KEY));
	}

	return pSnmpValue;
}

/**
 * Get SnmpValueDocs3 from the Set searched by indexed Key.
 */
CSnmpValueDocs3* CSnmpValueDocs3Set::get(const string& idx, const string& key) {
	return this->get(this->m_idx_list.getIdx(idx, key));
}

CSnmpValueDocs3* CSnmpValueDocs3Set::getDimDb() {

	int i;

	CSnmpValueDocs3* result = NULL;

	for(i = 0; i < size(); i++) {

		result = get(i);

		if (result && result->getSnmpOid().isDimdB()) {
			return result;
		}

	}

	return NULL;
}


void CSnmpValueDocs3Set::dumpInfo() {

	cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid() << " ---------- " << endl;

	for(int i = 0; i < size(); i++) {

		string dumpSnmpValue;

		m_set[i].dumpStrInfo(dumpSnmpValue);

		cout << "SnmpValueDocs3[" << i << "] " << dumpSnmpValue;
		cout << endl;

		//m_set[i].dumpInfo();

	}

}

/**
 * Dump all Docs3 SnmpValues from the list separated by separator
 */
void CSnmpValueDocs3Set::dumpSnmpValueListInfo(const string& separator, string& result) {

	for (int i = 0; i < this->size(); i++) {

		CSnmpValueDocs3* pSnmpValue = this->get(i);

		if (pSnmpValue) {
			pSnmpValue->dumpStrInfo(result);
			result += separator;
		}

	}
}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */


