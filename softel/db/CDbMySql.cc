/*
 * CDbMySql.cc
 *
 *  Created on: 27.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CDbMySql.h"
#include <softel/db/commonDefs.h>
#include <softel/core/commonDefs.h>
#include <softel/core/error/CErrorSql.h>

using namespace std;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::LOCK;

namespace SF {
namespace DB {

CDbMySql::CDbMySql(const string& host, const string& dbname, const string& user, const string& pass)
	: BDbSqlBase::BDbSqlBase(host, dbname, user, pass) {

	this->init();
}

CDbMySql::~CDbMySql() {
	mysql_close(m_db);
}

/**
 * Inialize MySql connection using stored already db account
 */
bool CDbMySql::init() {

	WriteSynchronized wl(*this);

	m_db = mysql_init(NULL);

	if (!m_db) {
		stringstream errorMessage;
		errorMessage << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__;
		errorMessage << " mysql_init failed";
		throw CErrorSql(errorMessage.str(), 0);
	}

	if (!mysql_real_connect(m_db,
							this->m_row_db.getColumn(KEY_HOST).c_str(),
							this->m_row_db.getColumn(KEY_USER).c_str(),
							this->m_row_db.getColumn(KEY_PASS).c_str(),
							this->m_row_db.getColumn(KEY_DBNAME).c_str(), 0, NULL, 0)) {
		throw CErrorSql(mysql_error(m_db), mysql_errno(m_db));
	}

	m_Res = NULL;

	m_date_created.setNow();

	return true;
}

/**
 * Should test alive-ness of the db connection
 * perhaps with dummy select execution
 */
void CDbMySql::testAlive() {
	m_is_alive = true;
}

/**
 * Reconnect(refresh) the db connection.
 */
void CDbMySql::refresh() {

	{
		WriteSynchronized wl(*this);

		if (m_db) {
			mysql_close(m_db);
		}
	}

	init();
}

/**
 * Execute SQL query.
 */
bool CDbMySql::doExecute(const string& sqlQuery) {

	WriteSynchronized wl(*this);

	if (mysql_query(m_db, sqlQuery.c_str()) != 0) {
		throw CErrorSql(mysql_error(m_db), sqlQuery, mysql_errno(m_db));
	}

	setAffectedRows();

	return true;
}

/**
 * Execute INSERT and set/return insert id.
 */
long CDbMySql::doInsert(const string& sqlQuery) {

	if (doExecute(sqlQuery)) {

		m_insert_id = mysql_insert_id(m_db);

	} else {
		m_insert_id = 0;
	}

	return m_insert_id;
}

/**
 * Execute UPDATE.
 */
long CDbMySql::doUpdate(const string& sqlQuery) {

	doExecute(sqlQuery);

	return this->getAffectedRows();
}

/**
 * Do Select and fill CDataSet with the result set.
 */
bool CDbMySql::doSelect(const string& sqlQuery, CDataSet& ds) {

	WriteSynchronized wl(*this);

	if (mysql_query(m_db, sqlQuery.c_str()) != 0) {
		throw CErrorSql(mysql_error(m_db), sqlQuery.c_str(), mysql_errno(m_db));
	}

	initDataSet(ds);

	fillDataSet(ds);

	mysql_free_result(m_Res);

	return true;
}

/**
 * Initialize Data Set with the column properties.
 */
void CDbMySql::initDataSet(CDataSet& ds) {

	MYSQL_FIELD *fields;

	m_Res = mysql_store_result(m_db);

	m_NumFields = mysql_num_fields(m_Res);

	fields = mysql_fetch_fields(m_Res);

	for (int i = 0; i < m_NumFields; i++) {
		ds.addColumn(fields[i].name);
	}
}

/**
 * Start MySql library before any threads are involved.
 * Should call finally endLibrary when finished.
 */
void CDbMySql::startLibrary() {
	 mysql_library_init(0, NULL, NULL);
}
void CDbMySql::endLibrary() {
	 mysql_library_end();
}
/**
 * Fill DataSet with rows from select query result set.
 */
void CDbMySql::fillDataSet(CDataSet& ds) {

	vector<string> rowValues;
	MYSQL_ROW row;

	while ((row = mysql_fetch_row(m_Res))) {

		unsigned long *lengths;

		lengths = mysql_fetch_lengths(m_Res);

		rowValues.clear();

		for (int i = 0; i < m_NumFields; i++) {
			rowValues.push_back(row[i] ? row[i] : "");
		}

		ds.addRow(rowValues);
	}

}

/**
 * Read counf of Affected Rows.
 * Should be called immediately after an sql execute.
 *
 */
void CDbMySql::setAffectedRows() {
	this->m_affected_rows = mysql_affected_rows(this->m_db);
}

} /* namespace DB */
} /* namespace SF */

