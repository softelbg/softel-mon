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

#ifndef CSqlConnPool_H
#define CSqlConnPool_H

#include <stdio.h>
#include <string>
#include <vector>

#include "CMySqlDB.h"


namespace SF {
namespace DB {

#define DEFAULT_POOL_SIZE (10);

class CSqlConnPool
{
public:
	CSqlConnPool(	const std::string& dbHost,
					const std::string& dbUserName,
					const std::string& dbPass,
					const std::string& dbName
				);

	~CSqlConnPool();

	void addMySqlConn();

	void startMySqlPool();
	void startMySqlPool(int poolSize);
	void clearMySqlPool();

	CMySqlDB* getMySqlConn(const int i);
	CMySqlDB* getMySqlConn();
	CMySqlDB* getNextMySqlConn();
	void freeMySqlConn(CMySqlDB*);

private:
	std::string m_dbHost;
	std::string m_dbUserName;
	std::string m_dbPass;
	std::string m_dbName;
	
	std::vector<CMySqlDB*> 	m_mysqlPool;
	std::vector<bool>		m_mysqlPoolStatus;
	int					m_mysqlPoolIndex;
	int					m_mysqlPoolIndexNext;

};

} /* namespace DB */
} /* namespace SF */

#endif
