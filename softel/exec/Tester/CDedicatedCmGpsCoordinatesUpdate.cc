/*
 * CDedicatedCmGpsCoordinatesUpdate.cc
 *
 *  Created on: 21.12.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CDedicatedCmGpsCoordinatesUpdate.h"
#include <softel/core/util/CStringUtil.h>
#include <softel/db/CDataRow.h>
#include <softel/core/error/CErrorSql.h>
#include "CTestSqlTools.h"

using namespace std;
using namespace SF::CORE::UTIL;
using namespace SF::DB;
using namespace SF::CORE::ERROR;

namespace SF {
namespace OSS {
namespace EXEC {

CDedicatedCmGpsCoordinatesUpdate::CDedicatedCmGpsCoordinatesUpdate() {
	// TODO Auto-generated constructor stub

}

CDedicatedCmGpsCoordinatesUpdate::CDedicatedCmGpsCoordinatesUpdate(
		const string& fileName) :
		BFileReaderBase(fileName) {

	//this->setListPropertiesNames();

	this->init();

	this->initCmList();

}

CDedicatedCmGpsCoordinatesUpdate::~CDedicatedCmGpsCoordinatesUpdate() {
	// TODO Auto-generated destructor stub
}

void CDedicatedCmGpsCoordinatesUpdate::processLine(const string& line) {

	cout << "Line:" << line << endl;

	// Check if first line (param list is still empty) and set the param list
	if (this->m_list_param_names.size() == 0) {

		this->setListPropertiesNames(line);

		return;
	}

	vector<string> listTokens;

	CStringUtil::tokenizeFindSubstring(line, ";", listTokens);

	CDataRow row;

	if (row.init(this->m_list_param_names, listTokens)) {

		this->m_list_cm.addRow(row);

	} else {
		cout << __FUNCTION__ << ": Error: size:";
		cout << this->m_list_param_names.size();
		cout << " size:" << listTokens.size();
		cout << endl;
	}

}

/**
 * Execute SQL list
 */
void CDedicatedCmGpsCoordinatesUpdate::executeSql() {

	CTestSqlTools sqlTools;

	CDataRow row;

	for (int i = 0; i < this->m_list_sql.size(); i++) {

		this->m_list_sql.getRow(i, row);

		string query = row.getColumn("update");

		cout << "SQL[" << i << "] = " << query << endl;

		try {

			long numAffectedRows = sqlTools.sqlInsert(query);

			if (numAffectedRows < 1) {
				cout << "Warn: affected:" << numAffectedRows << endl;
			}

		} catch (CErrorSql& e) {
			cout << "Error: " << e.getMessage() << endl;
		}

	}

}

void CDedicatedCmGpsCoordinatesUpdate::dumpInfo() {
	this->m_list_cm.dumpInfo();
	this->m_list_sql.dumpInfo();
}

/**
 * Prepare the SQL needed to update the CM ...
 */
void CDedicatedCmGpsCoordinatesUpdate::prepareSql(const CDataRow& row) {

	string strLng = row.getColumn("longitude");
	string strLat = row.getColumn("latitude");

	double lng = CStringUtil::convertGpsCoordinate(strLng);
	double lat = CStringUtil::convertGpsCoordinate(strLat);

	string macHex = row.getColumn("mac");
	string macId;

	CStringUtil::convertMac2Dec(macHex, macId);

	string address = row.getColumn("address2");
	//address += row.getColumn("address2") + " type:" + row.getColumn("type");

	stringstream ss;

	ss << "INSERT INTO oss_alert_cm_list ";
	ss << "(cmts_id, cm_mac, cm_address, cm_mac_hex, cm_latitude, cm_longitude) VALUES ";
	ss << "((SELECT cmts_id FROM oss_cm_latest WHERE cm_mac = " << macId << "), ";
	ss << macId << ", ";
	ss << " cm_address = \'" << address << "\', ";
	ss << " cm_mac_hex = \'" << macHex << "\', ";
	ss << " cm_latitude = " << lat << ", ";
	ss << " cm_longitude = " << lng;
	ss << ")";

	ss << " ON DUPLICATE KEY UPDATE ";
	ss << " cm_address = \'" << address << "\', ";
	ss << " cm_mac_hex = \'" << macHex << "\', ";
	ss << " cm_latitude = " << lat << ", ";
	ss << " cm_longitude = " << lng;

	CDataRow rowSql;
	rowSql.addColumn("update", ss.str());

	this->m_list_sql.addRow(rowSql);
}

/**
 * Prepare CM SQL Updates only for the active CMs from the List.
 * CM should has mac, gps coordinates and so on...
 */
void CDedicatedCmGpsCoordinatesUpdate::initCmList() {

	for(int i = 0; i < this->m_list_cm.size(); i++) {

		CDataRow row;

		this->m_list_cm.getRow(i, row);

		string param = row.getColumn("mac");

		if (param.size() > 0) {

			this->prepareSql(row);

		}


	}

}

/**
 * Set a list of properties.
 * Set List CM Columns.
 */
void CDedicatedCmGpsCoordinatesUpdate::setListPropertiesNames() {

	this->m_list_param_names.push_back("seq");
	this->m_list_param_names.push_back("model");
	this->m_list_param_names.push_back("mac");
	this->m_list_param_names.push_back("address1");
	this->m_list_param_names.push_back("address2");
	this->m_list_param_names.push_back("type");
	this->m_list_param_names.push_back("num_clients");
	this->m_list_param_names.push_back("date_set");
	this->m_list_param_names.push_back("note");
	this->m_list_param_names.push_back("latitude");
	this->m_list_param_names.push_back("longitude");

	this->m_list_cm.clear();
	this->m_list_cm.addColumn(this->m_list_param_names);

	this->m_list_sql.addColumn("update");
}

/**
 * Set a list of properties from a Line.
 *
 */
void CDedicatedCmGpsCoordinatesUpdate::setListPropertiesNames(
		const string& line) {

	this->m_list_param_names.clear();

	CStringUtil::tokenizeFindSubstring(line, ";", this->m_list_param_names);

	this->m_list_cm.clear();
	this->m_list_cm.addColumn(this->m_list_param_names);

	this->m_list_sql.addColumn("update");

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */


