/*
 * CDedicatedCmList.cc
 *
 *  Created on: 18.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CDedicatedCmList.h"
#include <softel/core/util/CStringUtil.h>
#include <softel/core/error/CErrorSql.h>
#include "CTestSqlTools.h"

using namespace std;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::ERROR;

namespace SF {
namespace OSS {
namespace EXEC {

CDedicatedCmList::CDedicatedCmList() :
	BFileReaderBase("cmlist.txt") {

	this->init();
}

CDedicatedCmList::~CDedicatedCmList() {
	// TODO Auto-generated destructor stub
}

CDedicatedCmList::CDedicatedCmList(const string& fileName) :
	BFileReaderBase(fileName) {

	this->init();
}

/**
 * Parse every line and make proper CM inserts into CM List.
 *
 */
void CDedicatedCmList::processLine(const string& line) {

	string sqlInsert;

	cout << "Line:" << line << endl;

	vector<string> listTokens;

	CStringUtil::tokenizeFindSubstring(line, ":::::", listTokens);

	if (listTokens.size() == 2) {

		this->prepareSql(listTokens, sqlInsert);

		this->m_list_sql.push_back(sqlInsert);

	} else {
		cout << "Error parse:" << line << endl;
	}


}

void CDedicatedCmList::prepareSql(const vector<string>& listParams,
		string& sqlResult) {

	string cmMacHex = listParams[0];
	string cmMacId;

	CStringUtil::convertMac2Dec(cmMacHex, cmMacId);

	stringstream ss;

	ss << "INSERT INTO oss_alert_cm_list ";
	ss << "(cmts_id, cm_mac, cm_mac_hex, cm_address) VALUES ";
	ss << "((SELECT cmts_id FROM oss_cm_latest WHERE cm_mac = " << cmMacId << "), ";
	ss << cmMacId << ", ";
	ss << "\'" << cmMacHex << "\', ";
	ss << "\'" << listParams[1] << "\')";

	sqlResult = ss.str();

}

/**
 * Execute already prepared SQL Inserts List.
 */
void CDedicatedCmList::executeSqlList() {

	CTestSqlTools sqlTools;

	for (unsigned int i = 0; i < this->m_list_sql.size(); i++) {

		stringstream ss;
		ss << this->m_list_sql[i];

		cout << "SQL[" << i << "] = " << ss.str() << endl;

		try {
			sqlTools.sqlExecute(ss);
		} catch (CErrorSql& e) {
			cout << "Error: " << e.getMessage() << endl;
		}

	}

}

void CDedicatedCmList::dumpInfo() {

	for (unsigned int i = 0; i < this->m_list_sql.size(); i++) {
		cout << "SQL[" << i << "] = " << this->m_list_sql[i] << endl;
	}

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */





