/*
 * CSqlThresholdTools.cc
 *
 *  Created on: 02.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSqlThresholdTools.h"

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::CMTS;

namespace SF {
namespace OSS {
namespace ALERT {

CSqlThresholdTools::CSqlThresholdTools() {
	// TODO Auto-generated constructor stub

}

CSqlThresholdTools::~CSqlThresholdTools() {
	// TODO Auto-generated destructor stub
}

/**
 * Select full Thresholds List.
 * Using Filter.
 */
void CSqlThresholdTools::sqlSelectThresholdList(const string& filter, CDataSet& ds) {

	stringstream ssSql;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;

	ssSql << "SELECT severity, cond, color, thresh_val, oid_label, col_key, oid_dimension, oss_cpe_oid_types.type_id ";
	ssSql << " FROM oss_thresholds, oss_thresholds_2_oid_types_map, oss_cpe_oid_types ";
	ssSql << " WHERE oss_thresholds.thresh_id = oss_thresholds_2_oid_types_map.thresh_id ";
	ssSql << " AND oss_thresholds_2_oid_types_map.oid_type_id = oss_cpe_oid_types.type_id ";

	if (filter.size()) {
		ssSql << " AND " << filter;
	}

	ssSql << " ORDER BY oss_cpe_oid_types.type_id, severity ";

	sqlSelect(ssSql, ds);

}

/**
 * Select Threshold List for CMTS CMs - from_cm = [0;1]
 *
 */
void CSqlThresholdTools::sqlSelectThresholdListCm(CDataSet& ds) {
	this->sqlSelectThresholdList(" from_cm < 2 ", ds);
}

/**
 * Select Threshold List for CMTS IFs - from_cm = 2
 *
 */
void CSqlThresholdTools::sqlSelectThresholdListIf(CDataSet& ds) {
	this->sqlSelectThresholdList(" from_cm = 2 ", ds);
}

/**
 * Select Thresholds for a Snmp Oid.
 * In common case there should be multiple Thresholds for a single Oid.
 *
 *	TODO: rework after tests...
 */
void CSqlThresholdTools::sqlSelectThresholdByOid(const CSnmpOid& oid, CDataSet& ds) {

	stringstream ssSql;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;

	ssSql << "SELECT severity, cond, color, thresh_val, oid_label, col_key, oid_dimension ";
	ssSql << " FROM oss_thresholds, oss_thresholds_2_oid_types_map, oss_cpe_oid_types ";
	ssSql << " WHERE oss_thresholds.thresh_id = oss_thresholds_2_oid_types_map.thresh_id ";
	ssSql << " AND oss_thresholds_2_oid_types_map.oid_type_id = oss_cpe_oid_types.type_id ";
	ssSql << " AND oss_cpe_oid_types.type_id = " << oid.getOidId();

	sqlSelect(ssSql, ds);

}

/**
 * Select OID List using filter.
 * Make proper CSnmpOids list.
 *
 */
bool CSqlThresholdTools::selectOidListFiltered(const string& filter, vector<CSnmpOid>& oidList) {

	CDataSet dsOidList;
	CDataRow row;
	stringstream ssSql;

	string colName;
	string colValue;

	ssSql << "SELECT * ";
	ssSql << " FROM " << TABLE_OSS_CPE_OID_TYPES;

	if (filter.size() > 0) {
		ssSql << " WHERE " << filter << " ";
	}

	ssSql << " ORDER BY oid_dimension ";

	sqlSelect(ssSql, dsOidList);

	oidList.clear();

	dsOidList.resetRowPos();

	while(dsOidList.getNextRow(row)) {

		CSnmpOid snmpOid;

		row.resetColPos();

		while(row.getNextColumn(colName, colValue)) {

			snmpOid.addProperty(colName, colValue);
		}

		oidList.push_back(snmpOid);
	}

	return true;
}

bool CSqlThresholdTools::selectOidList(vector<CSnmpOid>& oidList) {
	return selectOidListFiltered("", oidList);
}

/**
 * Select Full CM latest values from the Latest memory table.
 */
void CSqlThresholdTools::selectCmLatest(CDataSet& ds) {
	selectCmLatest(" unix_timestamp(now()) - dumpdate < 1800 AND cm_status = 6 ", ds);
}

void CSqlThresholdTools::selectCmLatest(const string& filter, CDataSet& ds) {

	stringstream ssSql;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;

	ssSql << "SELECT * ";
	ssSql << " FROM " << TABLE_OSS_CM_LATEST << ", " << TABLE_OSS_CM_LIST;
	ssSql << " WHERE " << TABLE_OSS_CM_LATEST << ".cm_mac = " << TABLE_OSS_CM_LIST << ".cm_mac ";
	ssSql << " AND " << TABLE_OSS_CM_LATEST << ".cmts_id = " << TABLE_OSS_CM_LIST << ".cmts_id ";

	if (filter.size()) {
		ssSql << " AND " << filter << " ";
	}

	sqlSelect(ssSql, ds);
}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

