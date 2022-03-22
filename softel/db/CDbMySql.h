/*
 * CDbMySql.h
 *
 * MySql Adapter.
 *
 *  Created on: 27.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CDBMYSQL_H_
#define CDBMYSQL_H_

#include <mysql/mysql.h>
#include <string>
#include <sstream>

#include "BDbSqlBase.h"


namespace SF {
namespace DB {

class CDbMySql: public BDbSqlBase {
public:
	CDbMySql(	const std::string& host,
				const std::string& dbname,
				const std::string& user,
				const std::string& pass);
	virtual ~CDbMySql();

	bool init();
	static void startLibrary();
	static void endLibrary();

	virtual bool doSelect(const std::string& sqlQuery, CDataSet& ds);
	virtual bool doExecute(const std::string& sqlQuery);
	virtual long doInsert(const std::string& sqlQuery);
	virtual long doUpdate(const std::string& sqlQuery);

	virtual void testAlive();
	virtual void refresh();

private:

	void setAffectedRows();

	void initDataSet(CDataSet& ds);
	void fillDataSet(CDataSet& ds);

	int 	m_NumFields;

	MYSQL *m_db;
	MYSQL_RES *m_Res;

};

} /* namespace DB */
} /* namespace SF */

#endif /* CDBMYSQL_H_ */
