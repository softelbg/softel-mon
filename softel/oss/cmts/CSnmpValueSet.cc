/*
 * CSnmpValueSet.cc
 *
 *  Created on: 12.03.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSnmpValueSet.h"

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::UTIL;

namespace SF {
namespace OSS {
namespace CMTS {

CSnmpValueSet::CSnmpValueSet() {

	clear();

}

CSnmpValueSet::~CSnmpValueSet() {

}

void CSnmpValueSet::clear() {
	m_snmp_value_set.clear();
	m_idx_type_id.clear();
	m_idx_col_key.clear();
	m_current_snmp_value = 0;
}

int CSnmpValueSet::size() {
	return m_snmp_value_set.size();
}

/**
 * Validate Param Set.
 *
 * Current implementation checks for UpSNR > 0
 * and DwnSNR should exists and > 0.
 *
 * TODO: Check for proper conditions...
 */
bool CSnmpValueSet::isParamSetValid() {

	bool status = true;

	status = this->isParamUpSnrValid();
	status = status && this->isParamDwnSnrValid();

	return status;
}

/**
 * Add/Set Snmp Value to list.
 */
void CSnmpValueSet::setSnmpValue(const CSnmpOid& snmpOid, const string& rawSnmpValue) {

	m_it_idx_type_id = m_idx_type_id.find(snmpOid.getProperty(KEY_PARAM_OID_ID));

	// OID not existed in the Set, so add it
	if (m_it_idx_type_id == m_idx_type_id.end()) {

		CSnmpValue snmpValue(snmpOid, rawSnmpValue);
		int pos = m_snmp_value_set.size();

		m_idx_col_key[snmpOid.getProperty(KEY_PARAM_COL_KEY)] = pos;
		m_idx_type_id[snmpOid.getProperty(KEY_PARAM_OID_ID)] = pos;
		// Add some additional indexing in common idx
		m_idx_common[snmpOid.getProperty(KEY_PARAM_CHANNEL) + snmpOid.getProperty(KEY_PARAM_COL_KEY)] = pos;

		m_snmp_value_set.push_back(snmpValue);

	} else {
		// OID already existing - so just update the value
		m_snmp_value_set[m_it_idx_type_id->second].setSnmpValue(rawSnmpValue);
	}

}

/**
 * Search Key using Indexes.
 * TODO: Consider using only one index (common index)
 */
CSnmpValue* CSnmpValueSet::getSnmpValue(const string& key) {

	CSnmpValue* res = NULL;

	m_it_idx_col_key = m_idx_col_key.find(key);
	m_it_idx_type_id = m_idx_type_id.find(key);
	m_it_idx_common = m_idx_common.find(key);

	if (m_it_idx_type_id != m_idx_type_id.end()) {

		res = &(m_snmp_value_set[m_it_idx_type_id->second]);

	} else if(m_it_idx_col_key != m_idx_col_key.end()) {

		res = &(m_snmp_value_set[m_it_idx_col_key->second]);

	} else if(m_it_idx_common != m_idx_common.end()) {

		res = &(m_snmp_value_set[m_it_idx_common->second]);

	}

	return res;
}

CSnmpValue* CSnmpValueSet::getSnmpValue(const unsigned int pos) {

	CSnmpValue* res = NULL;

	if (pos < m_snmp_value_set.size()) {
		res = &(m_snmp_value_set[pos]);
	}

	return res;
}

void CSnmpValueSet::incSnmpValueIterator() {
	m_current_snmp_value++;
}

void CSnmpValueSet::resetSnmpValueIterator() {
	m_current_snmp_value = 0;
}

bool CSnmpValueSet::getNextSnmpValue(CSnmpValue** pSnmpVal) {

	bool flag = false;

	if (m_current_snmp_value < size()) {

		*pSnmpVal = &(m_snmp_value_set[m_current_snmp_value]);

		incSnmpValueIterator();

		flag = true;

	} else {
		flag = false;
	}

	return flag;
}

bool CSnmpValueSet::getNextSnmpValueFnCol(CSnmpValue** pSnmpVal) {

	bool flag = false;

	do {

		CSnmpValue* pCurrentSnmpVal;

		flag = getNextSnmpValue(&pCurrentSnmpVal);

		if (flag == false) {
			break;
		}

		const CSnmpOid& rSnmpOid = pCurrentSnmpVal->getSnmpOid();

		// Find Function, Column Keyed oid
		if (rSnmpOid.isFunction() && rSnmpOid.hasColumn()) {

			*pSnmpVal = pCurrentSnmpVal;

			break;
		}

	} while (flag);

	return flag;
}

bool CSnmpValueSet::getNextSnmpValueCW(CSnmpValue** pSnmpVal) {

	bool flag = false;

	do {

		CSnmpValue* pCurrentSnmpVal;

		flag = getNextSnmpValue(&pCurrentSnmpVal);

		if (flag == false) {
			break;
		}

		const CSnmpOid& rSnmpOid = pCurrentSnmpVal->getSnmpOid();

		// Find CW function
		if (rSnmpOid.isFunction() && rSnmpOid.isDimCW()) {

			*pSnmpVal = pCurrentSnmpVal;

			break;
		}

	} while (flag);

	return flag;
}

bool CSnmpValueSet::getNextSnmpValueErrorsCW(CSnmpValue** pSnmpVal) {

	bool flag = false;

	do {

		CSnmpValue* pCurrentSnmpVal;

		flag = getNextSnmpValueCW(&pCurrentSnmpVal);

		if (flag == false) {
			break;
		}

		const CSnmpOid& rSnmpOid = pCurrentSnmpVal->getSnmpOid();

		// Find Errors Counters only
		if (rSnmpOid.hasColumn()) {

			*pSnmpVal = pCurrentSnmpVal;

			break;
		}

	} while (flag);

	return flag;
}

/**
 * Copy Error Counters Row
 */
void CSnmpValueSet::setErrorCountersRow(CDataRow& row) {

	//cout << __FILE__ << ":" << __FUNCTION__ << " pid:" << getpid() << " CM:" << getCmIp() << endl;

	row.copyRow(m_rowErrorCounters);

}

/**
 * Calculate Corrected and Uncorrected error counters as %.
 *
 * % Correctable = [(Ec1 – Ec0)/ [(Eu1 – Eu0) + (Ec1 – Ec0) + (C1 – C0)]] * 100
 * % Uncorrectable = [(Eu1 – Eu0)/ [(Eu1 – Eu0) + (Ec1 – Ec0) + (C1 – C0)]] * 100
 */
bool CSnmpValueSet::calcErrorsCounters() {

	map<string, long> map_counter_values;
	map<string, long>::iterator it;

	CDataRow& row = m_rowErrorCounters;

	CSnmpValue* pSnmpValue;

	//cout << __FILE__ << ":" << __FUNCTION__ << " pid:" << getpid() << " CM:" << getCmIp() << endl;

	map_counter_values.clear();

	if (row.size() > 0) {

		//row.dumpInfo();

		resetSnmpValueIterator();

		while(getNextSnmpValueCW(&pSnmpValue)) {

			const CSnmpOid& rSnmpOid = pSnmpValue->getSnmpOid();

			string rColKey;
			string flagChn;

			rSnmpOid.getProperty(KEY_PARAM_CHANNEL, flagChn);

			// if there is no column key, should be generated one...
			if (rSnmpOid.hasColumn() == false) {
				rColKey = flagChn + "_unerrored";
			} else {
				rSnmpOid.getProperty(KEY_PARAM_COL_KEY, rColKey);
			}

			const string& rColVal = pSnmpValue->getValue();
			string prevColVal;

			row.getColumn(rColKey, prevColVal);

			long lCurrentVal = atol(rColVal.c_str());
			long lPrevVal = atol(prevColVal.c_str());
			long diffVal = lCurrentVal - lPrevVal;


			// TODO: Decide what to do with negative difference....
			if (diffVal < 0) {
				//cout << __FUNCTION__ << ":pid:" << getpid() << " ERROR: lCurrentVal:" << lCurrentVal << " lPrevVal:" << lPrevVal;
				//cout << " rColVal:" << rColVal << " prevColVal:" << prevColVal << endl;
				//pSnmpValue->dumpInfo();

				diffVal = 0;
			}

			map_counter_values[rColKey] = diffVal;
			map_counter_values[flagChn + "_sum"] += diffVal;

		//	cout << __FUNCTION__ << ":pid:" << getpid() << " ";
		//	cout << "rColKey:" << rColKey << " rColVal:" << rColVal << " ";
		//	cout << "prevColVal:" << prevColVal << " lCurrentVal:"
		//			<< lCurrentVal << " lPrevVal:" << lPrevVal << " ";
		//	cout << endl;

		}

	} else {

		//cout << "Row Empty" << endl;

	}

	/*
	// Check are there any new counter values
	if (map_counter_values.size() == 0) {

		//cout << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << " map_counter_values is Empty" << endl;

		return false;
	}
*/

	resetSnmpValueIterator();

	while(getNextSnmpValueErrorsCW(&pSnmpValue)) {

		double calcVal = 0;

		// Check are there any new counter values and calculate new value if any
		if (map_counter_values.size() > 0) {
			const CSnmpOid& rSnmpOid = pSnmpValue->getSnmpOid();

			string flagChn;
			string colKey;

			rSnmpOid.getProperty(KEY_PARAM_CHANNEL, flagChn);
			rSnmpOid.getProperty(KEY_PARAM_COL_KEY, colKey);

			long diffSum = map_counter_values[flagChn + "_sum"];
			long diffVal = map_counter_values[colKey];

			if (diffVal > 0 && diffSum > 0) {
				calcVal = (100 * ((double) diffVal)) / ((double) diffSum);
			}
		} else {
			//cout << "Error: no previous error counter records." << endl;
		}

		stringstream ssSnmpResult;
		ssSnmpResult << calcVal;

		//cout << __FUNCTION__ << ":pid:" << getpid() << " ip:" << getCmIp() << " mac:" << getCmMac();
		//cout << " calcVal:" << calcVal << " ssSnmpResult:" << ssSnmpResult.str() << endl;

		pSnmpValue->setSnmpValue(ssSnmpResult.str());

	//	cout << " -------   New Snmp Value   -----------" << endl;
		//pSnmpValue->dumpInfo();

		// Debug if too high error % is calculated...

/*
		if (calcVal > 50) {
			cout << __FUNCTION__ << ":pid:" << getpid() << " ip:" << getCmIp();
			cout << " mac:" << getCmMac() << " calcVal:" << calcVal << " ssSnmpResult:" << ssSnmpResult.str();
			cout << " diffSum:" << diffSum << " diffVal:" << diffVal << endl;

			int i;
  		        for(i = 0, it = map_counter_values.begin(); it != map_counter_values.end(); it++, i++) {

 		               cout << "val[" << i << "] is " << it->first << " => " << it->second << endl;

 		        }


			pSnmpValue->dumpInfo();
		}
*/


	}


	return true;
}

bool CSnmpValueSet::isParamDwnSnrValid() {
	return this->isParamValid(KEY_COL_KEY_DWN_SNR);
}

bool CSnmpValueSet::isParamUpSnrValid() {
	return this->isParamValid(KEY_COL_KEY_UP_SNR);
}

/**
 * Check a Param from the Set is Valid.
 * Currently checks if there is the param and if its value is not zero.
 */
bool CSnmpValueSet::isParamValid(const string& param) {

	bool status = true;
	CSnmpValue* pSnmpValue = NULL;

	pSnmpValue = getSnmpValue(param);

	if ((pSnmpValue == NULL)
			|| (pSnmpValue && pSnmpValue->getValue().compare("0") == 0)) {

		status = false;
	}

	return status;
}

void CSnmpValueSet::dumpInfo() {

	resetSnmpValueIterator();

	CSnmpValue* pSnmpValue;

	cout << __FILE__ << " ------------- Snmp Value Set size:" << size() << endl;

	while(getNextSnmpValue(&pSnmpValue)) {

		pSnmpValue->dumpInfo();

	}

}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

