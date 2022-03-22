/*
 * WorkItemSqlPost.cc
 *
 *  Created on: 27.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "WorkItemSqlPost.h"
#include <softel/core/error/CErrorSql.h>
#include <softel/core/system/CSystemTools.h>

using namespace std;
using namespace SF::CORE::ERROR;
using namespace SF::DB;

namespace SF {
namespace CORE {
namespace POOL {


WorkItemSqlPost::WorkItemSqlPost(const std::string& query,
		SF::CORE::Ptr<IDbPostCallBackInterface>& cb, SF::CORE::Ptr<SF::DB::BDbSqlBase>& c) :
	WorkItemQueryBase(query), m_callback(cb), m_db_conn(c) {
	// TODO Auto-generated constructor stub

}

WorkItemSqlPost::~WorkItemSqlPost() {
	// TODO Auto-generated destructor stub
}

void WorkItemSqlPost::run() {

	cout << __FILE__ << "::" << __FUNCTION__ << " id: " << getParam("id") << " running..." << endl;

	DbResult dbResult;

	try {
		cout << __FILE__ << "::" << __FUNCTION__ << " EXEC: " << m_sql_query << endl;

		dbResult.ps.setParam("SQL", this->m_sql_query);

		m_db_conn->doExecute(this->m_sql_query);

		cout << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << endl;

		dbResult.ps.setParam("AFFECTED_ROWS", m_db_conn->getAffectedRows());
		dbResult.ps.setParam("INSERT_ID", m_db_conn->getInsertId());

		m_callback->onFinished(dbResult);

	} catch (CErrorSql& e) {
		dbResult.ps.setParam("CErrorSql", e.getMessage());
		m_callback->onError(dbResult);
	} catch (BErrorBase& e) {
		dbResult.ps.setParam("BErrorBase", e.getMessage());
		m_callback->onError(dbResult);
	}

	m_db_conn->release();

	cout << __FILE__ << " id: " << getParam("id") << " finished" << endl;
}

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */
