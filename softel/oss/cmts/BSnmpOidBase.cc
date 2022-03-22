/*
 * BSnmpOidBase.cc
 *
 *  Created on: 04.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BSnmpOidBase.h"

#include <softel/core/system/CSystemTools.h>

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace OSS {
namespace CMTS {

BSnmpOidBase::BSnmpOidBase() {
	// TODO Auto-generated constructor stub

}

BSnmpOidBase::BSnmpOidBase(const CDataRow& row) {
	this->init(row);
}

BSnmpOidBase::~BSnmpOidBase() {
	// TODO Auto-generated destructor stub
}

/**
 * Initialize Snmp Oid from a row filled with snmp oid params.
 *
 */
void BSnmpOidBase::init(const CDataRow& row) {
	row.copyRow(this->m_oid_params);
}

void BSnmpOidBase::addProperty(const string& key, const string& value) {
	this->m_oid_params.addColumn(key, value);
}

string BSnmpOidBase::getProperty(const string& key) const {
	return this->m_oid_params.getColumn(key);
}

bool BSnmpOidBase::getProperty(const string& key, string& result) const {
	return this->m_oid_params.getColumn(key, result);
}

void BSnmpOidBase::setOidId(const int oid_id) {
	stringstream ss;
	ss << oid_id;
	setOidId(ss.str());
}
void BSnmpOidBase::setOidId(const char* oid_id) {
	setOidId(oid_id);
}
void BSnmpOidBase::setOidId(const string& oid_id) {
	this->addProperty(KEY_PARAM_OID_ID, oid_id);
}

void BSnmpOidBase::setSnmpOid(const char* pSnmpOid) {
	this->setSnmpOid(pSnmpOid);
}
void BSnmpOidBase::setSnmpOid(const string& snmpOid) {
	this->addProperty(KEY_PARAM_OID_SNMP, snmpOid);
}

string BSnmpOidBase::getSnmpOid() const {
	return getProperty(KEY_PARAM_OID_SNMP);
}

int BSnmpOidBase::getOidId() const {
	string oidId;

	getProperty(KEY_PARAM_OID_ID, oidId);

	return atoi(oidId.c_str());
}

string BSnmpOidBase::getOidIdStr() const {
	return getProperty(KEY_PARAM_OID_ID);
}

void BSnmpOidBase::dumpInfo() const {

		cout << "PID: " << CSystemTools::doGetPid() << " ";
		cout << "[" << getOidIdStr() << "] ";
		cout << "[" << getSnmpOid() << "]" << endl;

		this->m_oid_params.dumpInfo();

		cout << endl;

}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */



