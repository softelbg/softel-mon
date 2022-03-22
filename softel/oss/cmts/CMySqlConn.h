/*
 * CMySqlConn.h
 *
 * MySql main connection handler - should read credentials from ini file.
 *
 *  Created on: 21.07.2012
 *      Author: STAN
 *
 *		www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 *
 */

#ifndef CMYSQLCONN_H_
#define CMYSQLCONN_H_

#include "CCmtsFileIni.h"
#include "SqlTableNames.h"

#include <softel/db/CSqlDbFactory.h>
#include <softel/db/CMySqlDB.h>

namespace SF {
namespace OSS {
namespace CMTS {

class CMySqlConn {

public:

	SF::DB::CSqlDbFactory* getDbFactory();
	SF::DB::CSqlConnPool* getConnPool();

	static CMySqlConn* getInstance();

	void setNumConnections(int numConnections);

private:

	CMySqlConn();

	void initDbFactory();

	static CMySqlConn* m_pInstance;

	int	m_num_connections;

	SF::DB::CSqlDbFactory* m_pDbFactory;
	SF::DB::CSqlConnPool* m_pDbConnPool;
	SF::DB::CMySqlDB* m_pDbConn;

};


} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CMYSQLCONN_H_ */
