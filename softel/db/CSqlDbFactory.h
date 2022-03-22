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

#ifndef CSqlDbFactory_H
#define CSqlDbFactory_H

#include <stdio.h>
#include <string>

#include "CMySqlDB.h"
#include "CDbMySql.h"
#include "CDbPgSql.h"
#include "CSqlConnPool.h"

namespace SF {
namespace DB {

class CSqlDbFactory {
public:
	CSqlDbFactory(const std::string& dbHost, const std::string& dbUserName,
			const std::string& dbPass, const std::string& dbName);

	~CSqlDbFactory();

	CMySqlDB* newMySqlConn();
	CDbMySql* newDbConnMySql();
	CDbPgSql* newDbConnPgSql();

	CSqlConnPool* newConnPool();

private:
	std::string m_dbHost;
	std::string m_dbUserName;
	std::string m_dbPass;
	std::string m_dbName;

	CSqlConnPool* m_pDbConnPool;

	pthread_mutex_t m_mutex;
};

} /* namespace DB */
} /* namespace SF */

#endif
