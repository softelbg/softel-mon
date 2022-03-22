/*
 * CCmtsElement.cc
 *
 *  Created on: 13.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCmtsElement.h"

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::NET;
using namespace SF::CORE::UTIL;

namespace SF {
namespace OSS {
namespace CMTS {

CCmtsElement::CCmtsElement() :
	m_dbFactory(NULL), m_dbConn(NULL) {

}

CCmtsElement::~CCmtsElement() {

}

void CCmtsElement::resetSnmpValueIterator() {
	m_snmp_value_set.resetSnmpValueIterator();
}

bool CCmtsElement::getNextSnmpValue(CSnmpValue** pSnmpVal) {
	return m_snmp_value_set.getNextSnmpValue(pSnmpVal);
}

bool CCmtsElement::getNextSnmpValueCW(CSnmpValue** pSnmpVal) {
	return m_snmp_value_set.getNextSnmpValueCW(pSnmpVal);
}

bool CCmtsElement::getNextSnmpValueErrorsCW(CSnmpValue** pSnmpVal) {
	return m_snmp_value_set.getNextSnmpValueErrorsCW(pSnmpVal);
}

bool CCmtsElement::getNextSnmpValueFnCol(CSnmpValue** pSnmpVal) {
	return m_snmp_value_set.getNextSnmpValueFnCol(pSnmpVal);
}

void CCmtsElement::setEventMessage(const string& message) {
	setParam(KEY_PARAM_EVENT_MESSAGE, message);
}

void CCmtsElement::addEventMessage(const string& message) {
	setEventMessage(getParam(KEY_PARAM_EVENT_MESSAGE) + " " + message);
}

/**
 * Get a SnmpValue from a Docs3 value set.
 * Filter by oid and ifid.
 */
CSnmpValue* CCmtsElement::getSnmpValueDocs3(const CSnmpOid& oid,
		const string& if_id) {

	CSnmpValue* pSnmpValue = NULL;

	CSnmpValueDocs3* pSnmpValueDocs3 = this->m_snmp_value_set_docs3.get(oid);

	if (pSnmpValueDocs3) {
		pSnmpValue = pSnmpValueDocs3->getSnmpValue(if_id);
	}

	return pSnmpValue;
}

/**
 * Copy all parameters from the DataRow.
 */
void CCmtsElement::setParam(const CDataRow& row) {
	row.appendRow(this->m_row_params);
}

/**
 * Copy all params to a DataRow.
 */
void CCmtsElement::getParam(CDataRow& row) const {
	this->m_row_params.copyRow(row);
}

void CCmtsElement::setErrorCountersRow(CDataRow& row) {
	m_snmp_value_set.setErrorCountersRow(row);
}

bool CCmtsElement::calcErrorsCounters() {
	return m_snmp_value_set.calcErrorsCounters();
}


void CCmtsElement::setAccountDb(CSqlDbFactory* pDb) {
	m_dbFactory = pDb;
}

void CCmtsElement::setCmtsIp(const string& ip) {
	setParam(KEY_SNMP_OID_CM_CMTS_IP, ip);
}

void CCmtsElement::setCmtsId(const string& cmts_id) {
	setParam(KEY_SNMP_OID_CM_CMTS_ID, cmts_id);
	m_tools_sql.setCmtsId(cmts_id);
}

void CCmtsElement::setCmtsId(const int cmts_id) {

	stringstream ss;

	ss << cmts_id;

	setCmtsId(ss.str());
}

string CCmtsElement::getCmtsId() const {
	return getParam(KEY_SNMP_OID_CM_CMTS_ID);
}

string CCmtsElement::getCmtsIp() const {
	return getParam(KEY_SNMP_OID_CM_CMTS_IP);
}

/**
 * Set CMTS and CM Read Communities
 */
void CCmtsElement::setRCommunity(const string& cmts_rcommunity, const string& cm_rcommunity) {
	setParam(KEY_COL_CMTS_RCOMM, cmts_rcommunity);
	setParam(KEY_COL_CM_RCOMM, cm_rcommunity);
}
/**
 * Get CMTS and CM Read Communities
 */
void CCmtsElement::getRCommunity(string& cmts_rcommunity, string& cm_rcommunity) const {
	cmts_rcommunity = getParam(KEY_COL_CMTS_RCOMM);
	cm_rcommunity = getParam(KEY_COL_CM_RCOMM);
}
string CCmtsElement::getRCommunityCmts() const {
	return getParam(KEY_COL_CMTS_RCOMM);
}
string CCmtsElement::getRCommunityCm() const {
	return getParam(KEY_COL_CM_RCOMM);
}

CCmtsToolsSql& CCmtsElement::getSqlTools() {
	return m_tools_sql;
}

void CCmtsElement::setDbConn(CMySqlDB* pDb) {
	m_dbConn = pDb;
}
CMySqlDB* CCmtsElement::getDbConn() {
	return m_dbConn;
}

/**
 * Set/Get Params functionality.
 */
void CCmtsElement::setParam(const string& key, const string& val) {
	this->m_row_params.addColumn(key, val);
}
void CCmtsElement::setParam(const CSnmpOid& snmpOid, const string& val) {
	setSnmpValue(snmpOid, val);
}

string CCmtsElement::getParam(const string& key) const {
	return this->m_row_params.getColumn(key);
}
int CCmtsElement::getParamCount() {
	return this->m_row_params.size();
}

CSnmpValue* CCmtsElement::getSnmpValue(const string& key) {
	return m_snmp_value_set.getSnmpValue(key);
}

/**
 * Get an SNMP Value param.
 */
bool CCmtsElement::getSnmpValue(const string& key, string& value) {

	CSnmpValue* snmpVal = getSnmpValue(key);

	if (snmpVal != NULL) {
		value = snmpVal->getValue();
		return true;
	} else {
		return false;
	}
}

/**
 * Add/Set Snmp Value to list.
 */
void CCmtsElement::setSnmpValue(const CSnmpOid& snmpOid, const string& rawSnmpValue) {
	m_snmp_value_set.setSnmpValue(snmpOid, rawSnmpValue);
	setParam(snmpOid.getProperty(KEY_PARAM_OID_ID), rawSnmpValue);
}

void CCmtsElement::setSnmpOidList(vector<CSnmpOid>* val) {
	m_pSnmpOidList = val;
}

bool CCmtsElement::setDocs3SnmpValue(const CSnmpOid& oid, const string& if_id, const string& snmpValue) {
	return m_snmp_value_set_docs3.setDocs3SnmpValue(oid, if_id, snmpValue);
}


} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

