/*
 * DbPool.cc
 *
 *  Created on: 16.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "DbPool.h"
#include <iostream>
#include <softel/core/commonDefs.h>
#include <softel/db/commonDefs.h>

#include <softel/db/BDbSqlBase.h>
#include <softel/db/CDbMySql.h>
#include <softel/db/CDbPgSql.h>

#include <softel/core/pool/WorkItemBase.h>
#include <softel/core/pool/WorkItemSqlPost.h>

#include <softel/sim/SimFileIni.h>

using namespace std;
using namespace SF::CORE;
using namespace SF::CORE::LOCK;
using namespace SF::CORE::ERROR;
using namespace SF::DB;

namespace SF {
namespace CORE {
namespace POOL {

DbPool::DbPool(enum DbType t, size_t numconn) :
	m_type(t), m_num_connections(numconn), m_flag_initialized(false) {

	m_thread_pool.reset(new ThreadPoolAsio(3));
}

DbPool::DbPool(enum DbType t, size_t numconn, Ptr<ThreadPoolAsio>& threadPool) :
	m_type(t), m_num_connections(numconn), m_flag_initialized(false) {

	m_thread_pool = threadPool;
}

DbPool::~DbPool() {
	// TODO Auto-generated destructor stub
}

void DbPool::clear() {
	m_conn_pool.clear();
	m_flag_initialized = false;
	m_used_conn = 0;
	m_next_conn = 0;
}

size_t DbPool::size() const {
	return m_conn_pool.size();
}

/**
 * Init db from simd.ini file
 * according to db type.
 */
void DbPool::init() {

	string dbType;

	switch(m_type) {
	case DbPool::MySql:
		dbType = "MySql";
		break;
	case DbPool::Postgresql:
		dbType = "PgSql";
		break;
	default:
		assert(false);
	}

	// Read db info from simd.ini file
	const std::string host = SF::SIM::SimFileIni::getInstance()->get(dbType, "host");
	const std::string user = SF::SIM::SimFileIni::getInstance()->get(dbType, "user");
	const std::string pass = SF::SIM::SimFileIni::getInstance()->get(dbType, "pass");
	const std::string dbname = SF::SIM::SimFileIni::getInstance()->get(dbType, "database");

	init(host, dbname, user, pass);
}

void DbPool::init(const std::string& host, const std::string& dbname,
	const std::string& user, const std::string& pass) {

	WriteSynchronized wlock(*this);

	clear();

	m_date_init.setNow();

	for(size_t i = 0; i < m_num_connections; i++) {

		SF::CORE::Ptr<SF::DB::BDbSqlBase> pDb;

		switch(m_type) {
			case DbPool::MySql:
				pDb.reset(new CDbMySql(host, dbname, user, pass));
				break;
			case DbPool::Postgresql:
				pDb.reset(new CDbPgSql(host, dbname, user, pass));
				break;
			default:
				assert(false);
		}

		pDb->setId(i);
		pDb->setPeriodValid(300); // Refreshing Period
		pDb->testAlive();
		pDb->setUsed(false);

		m_conn_pool.push_back(pDb);
	}

	m_flag_initialized = true;
}

/**
 * Frequently called in some scheduler. Used for maintenance and refreshing.
 * Reconnecting the connections...
 *
 */
void DbPool::refresh() {

	//cout << endl << __FILE__ << "::" << __FUNCTION__ <<  endl;

	WriteSynchronized wlock(*this);

	for(size_t i = 0; i < m_conn_pool.size(); i++) {
		if (m_conn_pool[i]->isExpired() && m_conn_pool[i]->isFree()) {
			m_conn_pool[i]->refresh();
		}
	}
}

/**
 * Post a Single SQL Query async. Usefull for queries which are not needed to be in specific order.
 *
 */
void DbPool::post(const string& query, SF::CORE::Ptr<IDbPostCallBackInterface>& cb) {
	assert(isInitialized());

	cb->onPost();

	cout << __FUNCTION__ << " SQL: " << query << endl;

	Ptr<BDbSqlBase> pDbConn;
	getConn(pDbConn);

	Ptr<WorkItemBase> wiSqlPost(new WorkItemSqlPost(query, cb, pDbConn));
	wiSqlPost->setParam("id", this->toHash());
	m_thread_pool->postWorkItem(wiSqlPost);
}

/**
 * Execute Single Query in sync.
 */
bool DbPool::execute(const string& query, DbResult& result) {
	assert(isInitialized());

	Ptr<BDbSqlBase> pDbConn;
	getConn(pDbConn);

	bool flagResult = true;

	try {
		result.ps.setParam("SQL", query);

		pDbConn->doExecute(query);

		result.ps.setParam("AFFECTED_ROWS", pDbConn->getAffectedRows());
		result.ps.setParam("INSERT_ID", pDbConn->getInsertId());

	} catch (CErrorSql& e) {
		result.ps.setParam("CErrorSql", e.getMessage());
		flagResult = false;
	} catch (BErrorBase& e) {
		result.ps.setParam("BErrorBase", e.getMessage());
		flagResult = false;
	}

	pDbConn->release();

	return flagResult;
}

/**
 * Do SELECT in sync.
 */
bool DbPool::doSelect(const std::string& query, DbResult& result) {
	assert(isInitialized());

	Ptr<BDbSqlBase> pDbConn;
	getConn(pDbConn);

	bool flagResult = true;

	try {
		result.ps.setParam("SQL", query);

		result.ds.clear();

		pDbConn->doSelect(query, result.ds);

		result.ps.setParam("AFFECTED_ROWS", pDbConn->getAffectedRows());

	} catch (CErrorSql& e) {
		result.ps.setParam("CErrorSql", e.getMessage());
		flagResult = false;
	} catch (BErrorBase& e) {
		result.ps.setParam("BErrorBase", e.getMessage());
		flagResult = false;
	}

	pDbConn->release();

	return flagResult;
}

size_t DbPool::getConnIdx() {
	m_next_conn++;
	if (m_next_conn < 0 || m_next_conn >= m_conn_pool.size()) {
		m_next_conn = 0;
	}

	return m_next_conn;
}

/**
 * Get free connection...
 *
 * Here should be considered a good selection approach:
 * Currently is searching till a not-used connection is found.
 */
bool DbPool::getConn(Ptr<BDbSqlBase>& c) {

	ReadSynchronized rlock(*this);

	while(true) {
		size_t connIdx = getConnIdx();
		c = m_conn_pool[connIdx];
		if (c->isFree()) {
			c->reserve();
			break;
		}
	}

	return true;
}

Ptr<BDbSqlBase> DbPool::getConn() {
	Ptr<BDbSqlBase> pDbConn;
	getConn(pDbConn);
	return pDbConn;
}

string DbPool::toString() const {
	stringstream ss;

	ss << "type:" << typeid(this).name();
	ss << " size:" << m_num_connections;
	ss << " used:" << m_used_conn;
	ss << " " << endl;

	return ss.str();
}

string DbPool::toXml() const {
	stringstream ss;

	size_t numConn = m_conn_pool.size();

	ss << "<type>" << typeid(this).name() << "</type>" << endl;
	ss << "<size>" << m_num_connections << "</size>" << endl;
	ss << "<sizep>" << numConn << "</sizep>" << endl;

	size_t numFree = 0;
	for(size_t i = 0; i < numConn; i++) {
		if (m_conn_pool[i]->isFree()) {
			numFree++;
		}

		ss << "<conn " << i << " free>" << m_conn_pool[i]->isFree() << "</conn>" << endl;
		ss << "<conn " << i << " elapsed>" << m_conn_pool[i]->getDateCreated().getElapsed();
		ss << "</conn>" << endl;
	}

	//m_used_conn = numConn - numFree;
	ss << "<used>" << m_used_conn << "</used>" << endl;
	ss << "<free>" << numFree << "</free>" << endl;

	return ss.str();
}

void DbPoolPostCallBackBase::onPost() {
	cout << endl << __FUNCTION__ << endl;
}

void DbPoolPostCallBackBase::onError(DbResult& result) {
	cout << endl << __FUNCTION__ << "  -------------------" << endl;
	result.ds.dumpInfo();
	result.ps.dumpInfo();
	cout << __FUNCTION__ << "  -------------------" << endl << endl;

}

void DbPoolPostCallBackBase::onFinished(DbResult& result) {
	cout << endl << __FUNCTION__ << "  -------------------" << endl;
	result.ds.dumpInfo();
	result.ps.dumpInfo();
	cout << __FUNCTION__ << "  -------------------" << endl << endl;
}

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */
