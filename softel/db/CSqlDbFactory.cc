/**
 * DB Factory.
 * Holds DB Account and produce new connections.
 *
 *	@author sgeorgiev
 *
 *		www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
*/

#include "CSqlDbFactory.h"

using namespace std;

namespace SF {
namespace DB {

CSqlDbFactory::CSqlDbFactory(	const string& dbHost,
								const string& dbUserName,
								const string& dbPass,
								const string& dbName
							)
{
	m_dbHost = dbHost;
	m_dbUserName = dbUserName;
	m_dbPass = dbPass;
	m_dbName = dbName;
}


CSqlDbFactory::~CSqlDbFactory()
{
}

/**
 * Create new MySql connection.
 * TODO: Obsolete - should use newDbConnMySql instead
 * 			and create CDbMySql connections...
 */
CMySqlDB* CSqlDbFactory::newMySqlConn() {

	//critical section - locked by mutex
	//::pthread_mutex_lock(&m_mutex);

	CMySqlDB* pConn = new CMySqlDB(	m_dbHost,
									m_dbUserName,
									m_dbPass,
									m_dbName
									);

	//::pthread_mutex_unlock(&m_mutex);
	//end of critical section

	return pConn;
}

/**
 * Create new MySql connection.
 */
CDbMySql* CSqlDbFactory::newDbConnMySql() {

	CDbMySql* pConn = new CDbMySql(m_dbHost, m_dbName, m_dbUserName, m_dbPass);

	return pConn;
}

/**
 * Create new PostgreSql connection.
 */
CDbPgSql* CSqlDbFactory::newDbConnPgSql() {

	CDbPgSql* pConn = new CDbPgSql(m_dbHost, m_dbName, m_dbUserName, m_dbPass);

	return pConn;
}

/**
 * Create new SQL Connection Pool.
 */
CSqlConnPool* CSqlDbFactory::newConnPool() {

	CSqlConnPool* pConnPool = new CSqlConnPool(	m_dbHost,
												m_dbUserName,
												m_dbPass,
												m_dbName
												);


	return pConnPool;
}

} /* namespace DB */
} /* namespace SF */


