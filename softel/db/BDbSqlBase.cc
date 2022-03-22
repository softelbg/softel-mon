/*
 * BDbSqlBase.cc
 *
 *  Created on: 03.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BDbSqlBase.h"
#include <softel/db/commonDefs.h>

#include <iostream>

using namespace std;

namespace SF {
namespace DB {

BDbSqlBase::BDbSqlBase() {
	// TODO Auto-generated constructor stub

}

BDbSqlBase::BDbSqlBase( const std::string& host,
				const std::string& dbname,
				const std::string& user,
				const std::string& pass) {

	init(host, dbname, user, pass);
}

BDbSqlBase::~BDbSqlBase() {
	// TODO Auto-generated destructor stub
}

long BDbSqlBase::getInsertId() {
	return this->m_insert_id;
}

bool BDbSqlBase::init(const string& host, const string& dbname,
		const string& user, const string& pass) {

	this->m_row_db.addColumn(KEY_HOST, host);
	this->m_row_db.addColumn(KEY_DBNAME, dbname);
	this->m_row_db.addColumn(KEY_USER, user);
	this->m_row_db.addColumn(KEY_PASS, pass);

	return true;
}

long BDbSqlBase::getAffectedRows() {
	return this->m_affected_rows;
}

void BDbSqlBase::dumpInfo() const {
	m_row_db.dumpInfo();
}

} /* namespace DB */
} /* namespace SF */
