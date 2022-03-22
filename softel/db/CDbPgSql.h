/*
 * CDbPgSql.h
 *
 *	PostgreSQL Wrapper
 *
 *	TODO: Concrete Implementation pending...
 *
 *  Created on: 03.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CDBPGSQL_H_
#define CDBPGSQL_H_

#include "BDbSqlBase.h"

namespace SF {
namespace DB {

class CDbPgSql: public BDbSqlBase {
public:
	CDbPgSql(const std::string& host, const std::string& dbname, const std::string& user, const std::string& pass);
	virtual ~CDbPgSql();

	bool init();

	virtual bool doSelect(const std::string& sqlQuery, CDataSet& ds);
	virtual bool doExecute(const std::string& sqlQuery);
	virtual long doInsert(const std::string& sqlQuery);
	virtual long doUpdate(const std::string& sqlQuery);

	virtual void testAlive();
	virtual void refresh();
};

} /* namespace DB */
} /* namespace SF */

#endif /* CDBPGSQL_H_ */
