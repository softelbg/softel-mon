/*
 * CSqlToolsBase.cc
 *
 *	Base MySql tools
 *
 *	TODO: Refactor...
 *
 *  Created on: 01.08.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSqlToolsBase.h"

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::UTIL;

namespace SF {
namespace OSS {
namespace CMTS {

CSqlToolsBase::CSqlToolsBase() {

	this->init(10);

}

CSqlToolsBase::CSqlToolsBase(int countConn) {

	this->init(countConn);

}

CSqlToolsBase::~CSqlToolsBase() {

	//delete m_pDb;

}

/**
 * Initialize connection pool and a default mysql connection
 */
void CSqlToolsBase::init(int countConn) {

	CMySqlConn::getInstance()->setNumConnections(countConn);

	this->m_pDb = CMySqlConn::getInstance()->getConnPool()->getNextMySqlConn();

}

/**
 * Execute SQL. Use new db connection.
 *
 */
bool CSqlToolsBase::sqlExecute(const string& query) {

	CMySqlDB* pDb = CMySqlConn::getInstance()->getConnPool()->getNextMySqlConn();

	bool result = sqlExecute(pDb, query);

	return result;
}

bool CSqlToolsBase::sqlExecute(const stringstream& query) {
	return this->sqlExecute(query.str());
}

/**
 * Execute SQL.
 *
 */
bool CSqlToolsBase::sqlExecute(CMySqlDB* pDb, const stringstream& query) {
	return pDb->doExecute(query.str());
}

bool CSqlToolsBase::sqlExecute(CMySqlDB* pDb, const string& query) {
	return pDb->doExecute(query);
}

/**
 * Execute INSERT and return insert id.
 */
long CSqlToolsBase::sqlInsert(const string& query) {

	CMySqlDB* pDb = CMySqlConn::getInstance()->getConnPool()->getNextMySqlConn();

	long result = pDb->doInsert(query);

	return result;

}
/**
 * Execute INSERT and return insert id.
 */
long CSqlToolsBase::sqlInsert(const stringstream& query) {
	return this->sqlInsert(query.str());
}
/**
 * Execute INSERT and return insert id.
 */
long CSqlToolsBase::sqlInsert(CMySqlDB* pDb, const stringstream& query) {
	return pDb->doInsert(query.str());
}

/**
 * Do SELECT and return fresh filled DataSet
 */
bool CSqlToolsBase::sqlSelect(CMySqlDB* pDb, const stringstream& query,
		CDataSet& ds) {

	ds.clear();

	return pDb->doSelect(query.str(), ds);
}

/**
 * Sql Update - should return num of affected rows.
 */
long CSqlToolsBase::sqlUpdate(const stringstream& query) {
	return this->sqlUpdate(query.str());
}

/**
 * Sql Update - should return num of affected rows.
 */
long CSqlToolsBase::sqlUpdate(const string& query) {

	CMySqlDB* pDb = CMySqlConn::getInstance()->getConnPool()->getNextMySqlConn();

	long result = pDb->doUpdate(query);

	return result;
}

bool CSqlToolsBase::sqlSelect(const stringstream& query, CDataSet& ds) {

	CMySqlDB* pDb = CMySqlConn::getInstance()->getConnPool()->getNextMySqlConn();

	bool result = sqlSelect(pDb, query, ds);

	return result;
}


} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */
