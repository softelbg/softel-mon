/*
 * CMySqlConn.cc
 *
 *  Created on: 21.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CMySqlConn.h"

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::UTIL;

namespace SF {
namespace OSS {
namespace CMTS {

CMySqlConn* CMySqlConn::m_pInstance = 0;

CMySqlConn::CMySqlConn() {

	initDbFactory();

}

CMySqlConn* CMySqlConn::getInstance() {

	if (m_pInstance == NULL) {
		m_pInstance = new CMySqlConn();
	}

	return m_pInstance;
}

void CMySqlConn::setNumConnections(int numConnections) {
	m_num_connections = numConnections;
}

void CMySqlConn::initDbFactory() {

	m_pDbFactory = new CSqlDbFactory(	CCmtsFileIni::getInstance()->get("MySQL", "Host"),
										CCmtsFileIni::getInstance()->get("MySQL", "User"),
										CCmtsFileIni::getInstance()->get("MySQL", "Pass"),
										CCmtsFileIni::getInstance()->get("MySQL", "Database")
									);

	m_pDbConnPool = NULL;
	m_num_connections = 1;

}

CSqlDbFactory* CMySqlConn::getDbFactory() {
	return m_pDbFactory;
}

CSqlConnPool* CMySqlConn::getConnPool() {

	if (m_pDbConnPool == NULL) {
		m_pDbConnPool = m_pDbFactory->newConnPool();
		m_pDbConnPool->startMySqlPool(m_num_connections);
	}

	return m_pDbConnPool;
}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

