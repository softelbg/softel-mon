/*
 * BDbSqlBase.h
 *
 *  Created on: 03.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BDBSQLBASE_H_
#define BDBSQLBASE_H_

#include "ISqlDbInterface.h"
#include <softel/core/pool/BConnectionBase.h>

namespace SF {
namespace DB {

class BDbSqlBase: virtual public ISqlDbInterface,
				  public SF::CORE::POOL::BConnectionBase {
public:
	BDbSqlBase();
	BDbSqlBase( const std::string& host,
				const std::string& dbname,
				const std::string& user,
				const std::string& pass);
	virtual ~BDbSqlBase();

	bool init(const std::string& host, const std::string& dbname,
			const std::string& user, const std::string& pass);

	long getInsertId();
	long getAffectedRows();

	void dumpInfo() const;

protected:
	long m_insert_id;
	long m_affected_rows;

	CDataRow m_row_db;

private:

};

} /* namespace DB */
} /* namespace SF */

#endif /* BDBSQLBASE_H_ */
