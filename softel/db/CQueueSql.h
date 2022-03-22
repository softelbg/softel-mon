/*
 * CQueueSql.h
 *
 *  Created on: 14.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CQUEUESQL_H_
#define CQUEUESQL_H_

#include <pthread.h>

#include <stdio.h>
#include <string>
#include <deque>

#include "CMySqlDB.h"
#include "CSqlConnPool.h"


namespace SF {
namespace DB {

#define NUM_DB_CONN 50

class CQueueSql {


public:

	CQueueSql(	const std::string& dbHost,
				const std::string& dbUserName,
				const std::string& dbPass,
				const std::string& dbName
			);

	~CQueueSql();

	void pushSql(const std::string& sqlStmt);
	std::string popSql();

	int countQueue();

	bool isEmpty();

	void clear();

private:

	std::string m_dbHost;
	std::string m_dbUserName;
	std::string m_dbPass;
	std::string m_dbName;

	CSqlConnPool& m_poolDbConn;

	std::deque<std::string> m_sqlQueue;

	pthread_mutex_t m_mutex;
};

} /* namespace DB */
} /* namespace SF */

#endif /* CQUEUESQL_H_ */
