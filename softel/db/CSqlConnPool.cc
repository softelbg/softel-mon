/**
 * SQL Connection Pool
 *
 *	@author sgeorgiev
 *
 *		www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
*/

#include "CSqlConnPool.h"

using namespace std;

namespace SF {
namespace DB {

CSqlConnPool::CSqlConnPool(		const string& dbHost,
								const string& dbUserName,
								const string& dbPass,
								const string& dbName
	)
{
	m_dbHost = dbHost;
	m_dbUserName = dbUserName;
	m_dbPass = dbPass;
	m_dbName = dbName;

	m_mysqlPool.clear();

	m_mysqlPoolIndex = 0;
	m_mysqlPoolIndexNext = 0;
}

CSqlConnPool::~CSqlConnPool()
{
	clearMySqlPool();
}

void CSqlConnPool::addMySqlConn() {

	CMySqlDB* pConn = new CMySqlDB(	m_dbHost.c_str(),
									m_dbUserName.c_str(),
									m_dbPass.c_str(),
									m_dbName.c_str());

	m_mysqlPool.push_back(pConn);
	m_mysqlPoolStatus.push_back(true);

}

void CSqlConnPool::startMySqlPool() {
	startMySqlPool(10);
}

void CSqlConnPool::startMySqlPool(int poolSize) {

	clearMySqlPool();

	for(int i = 0; i < poolSize; i++) {
		addMySqlConn();
	}

}

void CSqlConnPool::clearMySqlPool() {

	for(int i = 0; i < m_mysqlPool.size(); i++) {
		delete m_mysqlPool[i];
	}

	m_mysqlPool.clear();
	m_mysqlPoolStatus.clear();

	m_mysqlPoolIndex = 0;
}

/**
 * Get Next SQL Connection.
 * Just get next conn (round robin style)
 */
CMySqlDB* CSqlConnPool::getNextMySqlConn() {

	m_mysqlPoolIndexNext++;

	if (m_mysqlPoolIndexNext >= m_mysqlPool.size()) {
		m_mysqlPoolIndexNext = 0;
	}

	return m_mysqlPool[m_mysqlPoolIndexNext];
}

/**
 * Get Next SQL Connection.
 * TODO: Make it chose connection smarter....
 */
CMySqlDB* CSqlConnPool::getMySqlConn() {

	int i;

	for(i = 0; i < m_mysqlPool.size(); i++) {
		if (m_mysqlPoolStatus[i] == true) {
			break;
		}
	}

	if (i >= m_mysqlPool.size()) {
		addMySqlConn();
	}

	m_mysqlPoolIndex = i;

	return getMySqlConn(i);
}

/**
 * Get connection by index - make it reserved but do not check if is free...
 */
CMySqlDB* CSqlConnPool::getMySqlConn(const int i) {

	m_mysqlPoolStatus[i] = false;

	return m_mysqlPool[i];
}

/**
 * Free(Release) the connection.
 * Connection is not realy closed, but is marked as free.
 */
void CSqlConnPool::freeMySqlConn(CMySqlDB* pDb) {

	// TODO: Make better algo for searching...
	for(int i = 0; i < m_mysqlPool.size(); i++) {
		if (m_mysqlPool[i] == pDb) {
			m_mysqlPoolStatus[i] = true;
			break;
		}
	}

}

} /* namespace DB */
} /* namespace SF */

