/*
 * CDbPgSql.cc
 *
 *  Created on: 03.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CDbPgSql.h"
#include <softel/db/commonDefs.h>
#include <softel/core/commonDefs.h>
#include <softel/core/error/CErrorSql.h>

using namespace std;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::LOCK;

namespace SF {
namespace DB {

CDbPgSql::CDbPgSql(const string& host, const string& dbname, const string& user, const string& pass) {
	BDbSqlBase::init(host, dbname, user, pass);
}

CDbPgSql::~CDbPgSql() {
	// TODO Auto-generated destructor stub
}

bool CDbPgSql::init() {

	return false;
}

bool CDbPgSql::doSelect(const string& sqlQuery, CDataSet& ds) {
	return false;
}

bool CDbPgSql::doExecute(const string& sqlQuery) {
	return false;
}

long CDbPgSql::doInsert(const string& sqlQuery) {
	return false;
}

long CDbPgSql::doUpdate(const string& sqlQuery) {
	return false;
}

/**
 * Should test alive-ness of the db connection
 * perhaps with dummy select execution
 */
void CDbPgSql::testAlive() {
	m_is_alive = true;
}

/**
 * Reconnect(refresh) the db connection.
 */
void CDbPgSql::refresh() {

	WriteSynchronized wl(*this);

}


} /* namespace DB */
} /* namespace SF */


