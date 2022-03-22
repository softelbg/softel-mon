/*
 * CQueueSql.cc
 *
 *  Created on: 14.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CQueueSql.h"

using namespace std;

namespace SF {
namespace DB {

CQueueSql::CQueueSql(		const string& dbHost,
							const string& dbUserName,
							const string& dbPass,
							const string& dbName
	) : m_poolDbConn(dbHost, dbUserName, dbPass, dbName)
{

	m_dbHost = dbHost;
	m_dbUserName = dbUserName;
	m_dbPass = dbPass;
	m_dbName = dbName;

	m_poolDbConn.startMySqlPool(NUM_DB_CONN);

	pthread_mutex_init(&m_mutex, NULL);

}

CQueueSql::~CQueueSql() {

	clear();

	m_poolDbConn.clearMySqlPool();

}

/**
 * Clear Sql Queue.
 */
void CQueueSql::clear() {
	m_sqlQueue.clear();
}

/**
 * Push a SQL statement to the end of queue
 */
void CQueueSql::pushSql(const string& sqlStmt) {

	pthread_mutex_lock(&m_mutex);

	m_sqlQueue.push_back(sqlStmt);

	pthread_mutex_unlock(&m_mutex);

}

/**
 * Pop (and remove) a SQL statement from queue.
 */
string CQueueSql::popSql() {

	string strSql;

	pthread_mutex_lock(&m_mutex);

	if (m_sqlQueue.empty() == false) {
		strSql = m_sqlQueue.begin();
		m_sqlQueue.pop_front();
	}

	pthread_mutex_unlock(&m_mutex);

	return strSql;
}

/**
 * Check if the queue is empty
 */
bool CQueueSql::isEmpty() {

	bool result;

	pthread_mutex_lock(&m_mutex);

	result = m_sqlQueue.empty();

	pthread_mutex_unlock(&m_mutex);

	return result;
}

/**
 * Return size of the Queue
 */
int CQueueSql::countQueue() {

	int result;

	pthread_mutex_lock(&m_mutex);

	result = m_sqlQueue.size();

	pthread_mutex_unlock(&m_mutex);

	return result;
}

} /* namespace DB */
} /* namespace SF */


