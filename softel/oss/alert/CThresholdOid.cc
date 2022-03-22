/*
 * CThresholdOid.cc
 *
 *  Created on: 02.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CThresholdOid.h"

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::CMTS;

namespace SF {
namespace OSS {
namespace ALERT {

CThresholdOid::CThresholdOid(const CSnmpOid& oid) {

	this->m_snmp_oid = oid;

}

CThresholdOid::~CThresholdOid() {
	// TODO Auto-generated destructor stub
}

/**
 * Init the oid threshold list from DB.
 */
void CThresholdOid::init() {

	CDataRow row;

	this->clear();

	CSqlThresholdTools	sqlTools;

	sqlTools.sqlSelectThresholdByOid(this->m_snmp_oid, this->m_data_set);

	for(int i = 0; i < this->m_data_set.size(); i++) {

		this->m_data_set.getRow(i, row);

		this->add(row);
	}

}

/**
 * Init from a Data Set with multiple oid<->threshold rows.
 *
 */
void CThresholdOid::init(const CDataSet& ds) {

	CDataRow row;

	this->clear();

	for (int i = 0; i < ds.size(); i++) {

		ds.getRow(i, row);

		if (row.getColumn(KEY_PARAM_OID_ID).size() &&
			row.getColumn(KEY_PARAM_OID_ID).compare(this->m_snmp_oid.getOidIdStr()) == 0) {

			this->add(row);

		}

	}

}

string CThresholdOid::getOidId() const {
	return this->m_snmp_oid.getOidIdStr();
}

/**
 * Compare input value with current threshold list values.
 *
 */
void CThresholdOid::compare(const string& value, CThresholdResult& result) {

	int i = 0;

	// TODO: Assume that threshold list is sorted by severity from ordering of db select...
	for(i = 0; i < m_list_thresholds.size(); i++) {
		if (m_list_thresholds[i].compare(value)) {
			break;
		}
	}

	// If Found a Threshold meeting the condition use it, otherwise should set green one.
	if (i < m_list_thresholds.size()) {
		result.setThreshold(&m_list_thresholds[i]);

		result.setParam(PARAM_THRES_SEVERITY, m_list_thresholds[i].getSeverity());
		result.setParam(PARAM_THRES_COLOR, m_list_thresholds[i].getColor());
	} else {

		result.setParam(PARAM_THRES_SEVERITY, "100000");
		result.setParam(PARAM_THRES_COLOR, "green");
	}

	result.setSnmpOid(&this->m_snmp_oid);
	result.setParam(PARAM_THRES_VALUE, value);
	result.setParam(KEY_PARAM_COL_KEY, this->m_snmp_oid.getProperty(KEY_PARAM_COL_KEY));

}

void CThresholdOid::compare(const CSnmpValue& value, CThresholdResult& result) {
	compare(value.getValue(), result);
}

string CThresholdOid::getColKey() const {
	return this->m_snmp_oid.getProperty(KEY_PARAM_COL_KEY);
}

void CThresholdOid::dumpInfo() const {

	for (int i = 0; i < this->m_list_thresholds.size(); i++) {

		cout << "Oid: " << this->m_snmp_oid.getProperty(KEY_PARAM_COL_KEY) << " " << getOidId() << " ";

		this->m_list_thresholds[i].dumpInfo();

		cout << endl;
	}

}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */




