/*
 * WorkItemMySqlQuery.cc
 *
 *  Created on: 27.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "WorkItemMySqlQuery.h"
#include "CDbMySql.h"
#include <softel/sim/SimFileIni.h>
#include <softel/core/error/CErrorSql.h>
#include <softel/core/system/CSystemTools.h>

using namespace std;
using namespace SF::SIM;
using namespace SF::CORE::ERROR;

namespace SF {
namespace DB {

WorkItemMySqlQuery::WorkItemMySqlQuery(const std::string& sqlQuery) :
	WorkItemQueryBase(sqlQuery) {
	// TODO Auto-generated constructor stub

}

WorkItemMySqlQuery::~WorkItemMySqlQuery() {
	// TODO Auto-generated destructor stub
}

void WorkItemMySqlQuery::run() {

	cout << __FILE__ << "::" << __FUNCTION__ << " id: " << getParam("id") << " running..." << endl;

	int countTry = 0;

	do {
		countTry++;

		try {
			cout << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << endl;

			const std::string host = SimFileIni::getInstance()->get("MySQL", "host");
			const std::string user = SimFileIni::getInstance()->get("MySQL", "user");
			const std::string pass = SimFileIni::getInstance()->get("MySQL", "pass");
			const std::string dbName = SimFileIni::getInstance()->get("MySQL", "database");

			cout << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << endl;

			CDbMySql conn(host, dbName, user, pass);

			cout << __FILE__ << "::" << __FUNCTION__ << " EXEC: " << m_sql_query << endl;

			conn.doExecute(this->m_sql_query);

			cout << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << endl;

			this->setParam("AFFECTED_ROWS", conn.getAffectedRows());
			this->setParam("INSERT_ID", conn.getInsertId());

			break;

		} catch (CErrorSql& e) {
			this->setParam("CErrorSql", e.getMessage());
		} catch (BErrorBase& e) {
			this->setParam("BErrorBase", e.getMessage());
		}

		SF::CORE::SYSTEM::CSystemTools::doSleep(1);
	} while(countTry < 5);

	this->dumpInfo();

	SF::CORE::SYSTEM::CSystemTools::doSleep(2);
	cout << __FILE__ << " id: " << getParam("id") << " finished" << endl;
}

} /* namespace DB */
} /* namespace SF */
