/**
 * SNMP OID Representation
 *
 *	@author sgeorgiev
 *
 *		www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSnmpOid.h"

using namespace std;
using namespace SF::DB;

namespace SF {
namespace OSS {
namespace CMTS {

CSnmpOid::CSnmpOid(const CDataRow& row) :
	BSnmpOidBase(row) {

}

CSnmpOid::CSnmpOid(int oid_id) {
	setOidId(oid_id);
}

CSnmpOid::CSnmpOid(const string& oid_id) {
	setOidId(oid_id);
}

CSnmpOid::CSnmpOid(const string& snmp_oid, int oid_id) {
	setOidId(oid_id);
	setSnmpOid(snmp_oid);
}

CSnmpOid::CSnmpOid() {
}

CSnmpOid::~CSnmpOid() {
}


/**
 * Check if the Snmp Oid is for channel upstream
 *
 */
bool CSnmpOid::isUpstream() const {

	string param;

	if (getProperty(KEY_PARAM_CHANNEL, param) == false) {
		return false;
	}

	if (param.compare(CHANNEL_VAL_UP) == 0 || param.compare(CHANNEL_VAL_UNKNOWN) == 0) {
		return true;
	} else {
		return false;
	}
}

/**
 * Check if the Snmp Oid is for channel upstream strictly (not allowed 'x' value)
 *
 */
bool CSnmpOid::isUpstreamStrict() const {
	string param;

	if (getProperty(KEY_PARAM_CHANNEL, param) == false) {
		return false;
	}

	if (param.compare(CHANNEL_VAL_UP) == 0) {
		return true;
	} else {
		return false;
	}
}

/**
 * Check if the Snmp Oid is for channel downstream
 *
 */
bool CSnmpOid::isDownstream() const {

	string param;

	if (getProperty(KEY_PARAM_CHANNEL, param) == false) {
		return false;
	}

	if (param.compare(CHANNEL_VAL_DOWN) == 0 || param.compare(CHANNEL_VAL_UNKNOWN) == 0) {
		return true;
	} else {
		return false;
	}
}

/**
 * Check if the Snmp Oid is for channel downstream strictly (not allowed 'x' value)
 *
 */
bool CSnmpOid::isDownstreamStrict() const {
	string param;

	if (getProperty(KEY_PARAM_CHANNEL, param) == false) {
		return false;
	}

	if (param.compare(CHANNEL_VAL_DOWN) == 0) {
		return true;
	} else {
		return false;
	}
}

/**
 * Check if the Oid values are calculated as avg from all CMs.
 * Now the snmp oid is empty for these oids.
 */
bool CSnmpOid::isAvgFromCM() const {

	string param;

	if (getProperty(KEY_PARAM_OID_SNMP, param) == false) {
		return false;
	}

	if (param.size() > 0) {
		return false;
	} else {
		return true;
	}
}

/**
 * Check if the Oid values are calculated as avg from all CMs AND Is a Function.
 * Now the snmp oid is empty for these oids.
 */
bool CSnmpOid::isFnAvgFromCM() const {
	return (isAvgFromCM() && isFunction());
}

/**
 * Check if the Snmp Oid is a Function(means that snmp values should be stored)
 */
bool CSnmpOid::isFunction() const {

	string param;

	if (getProperty(KEY_PARAM_OID_FN, param) == false) {
		return false;
	}
 
	if (param.compare(VAL_IS_NOT_FN) == 0) {
		return false;
	} else {
		return true;
	}
}
/**
 * Check if the Snmp Oid has Column Key (means it should be added in function table).
 */
bool CSnmpOid::hasColumn() const {
	
	string param;

        if (getProperty(KEY_PARAM_COL_KEY, param) == false) {
                return false;
        }

	if (param.size() == 0) {
		return false;
	} else {
		return true;
	}
}

bool CSnmpOid::isDim(const string& dim) const {

	string param;

	if (getProperty(KEY_PARAM_OID_DIM, param) == false) {
		return false;
	}

	if (param.compare(dim) == 0) {
		return true;
	} else {
		return false;
	}
}

/**
 * Check if the Snmp Oid dimension is CW (code words).
 * Usually these are code word counters for unerrored, corrected, uncorrected...
 */
bool CSnmpOid::isDimCW() const {
	return isDim(KEY_PARAM_DIM_CW);
}

bool CSnmpOid::isDimdB() const {
	return isDim(KEY_PARAM_DIM_DB);
}

bool CSnmpOid::isDimdBm() const {
	return isDim(KEY_PARAM_DIM_DBMV);
}

/**
 * Check if the OID is Valid (should prio < 100)
 */
bool CSnmpOid::isValid() const {

	string strPrio;

	getProperty(KEY_PARAM_PRIO, strPrio);
	
	int prio = atoi(strPrio.c_str());
 
	if (strPrio.size() > 0 && prio < 100) {
		return true;
	} else {
		return false;
	}
}

/**
 * Check if the OID is for CM param or it is about IF.
 */
bool CSnmpOid::isCM() const {

	string strFromCm;

	getProperty(KEY_PARAM_FROM_CM, strFromCm);
	
	int fromCm = atoi(strFromCm.c_str());

	if (strFromCm.size() > 0 && fromCm < 2) {
		return true;
	} else {
		return false;
	}
}

/**
 * Check if the OID is for IF param.
 */
bool CSnmpOid::isIf() const {

	string strFromCm;

	getProperty(KEY_PARAM_FROM_CM, strFromCm);

	int fromCm = atoi(strFromCm.c_str());

	if (strFromCm.size() > 0 && fromCm == 2) {
		return true;
	} else {
		return false;
	}
}

bool CSnmpOid::isDimMbit() const {
	return isDim(KEY_PARAM_DIM_MBIT);
}

/**
 * Initialize Snmp Oid from DataBase.
 * Should read a row with current oid id.
 */
void CSnmpOid::initFromDB() {

	CMySqlDB* pDb = CMySqlConn::getInstance()->getConnPool()->getNextMySqlConn();

	CDataSet ds;
	stringstream sqlQuery;

	sqlQuery << "SELECT * FROM " << TABLE_OSS_CPE_OID_TYPES;
	sqlQuery << " WHERE type_id = " << this->getOidIdStr();

	pDb->doSelect(sqlQuery.str(), ds);

	if (ds.size() > 0) {
		CDataRow row;
		ds.getRow(0, row);
		this->init(row);
	}
}

string CSnmpOid::getColKey() const {
	return this->getProperty(KEY_PARAM_COL_KEY);
}

/**
 * Check is param for IF on Demand only - from_cm = 3
 */
bool CSnmpOid::isIfOnDemand() const {

	string strFromCm;

	getProperty(KEY_PARAM_FROM_CM, strFromCm);

	int fromCm = atoi(strFromCm.c_str());

	if (strFromCm.size() > 0 && fromCm == 3) {
		return true;
	} else {
		return false;
	}

}

/**
 * Dump Snmp Oid List
 *
*/
void CSnmpOid::dumpOidList(vector<CSnmpOid>& snmpOidList) {

	cout << __FUNCTION__ << " ----------- Snmp List size:" << snmpOidList.size() << " ----------" << endl;

	for(unsigned int i = 0; i < snmpOidList.size(); i++) {
		cout << "SnmpOid[" << i << "] ===> " << endl;
		snmpOidList[i].dumpInfo();
	}

}


} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */



