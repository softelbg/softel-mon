/*
 * WorkItemQueryBase.h
 *
 * Base Work Item for DB Queries. Currently only exec statements.
 *
 *  Created on: 27.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef WORKITEMQUERYBASE_H_
#define WORKITEMQUERYBASE_H_

#include <softel/core/pool/WorkItemBase.h>

namespace SF {
namespace DB {

/*
 * Base Work Item for DB Queries
 */
class WorkItemQueryBase: public SF::CORE::POOL::WorkItemBase {
public:
	WorkItemQueryBase(const std::string& sqlQuery);
	virtual ~WorkItemQueryBase();

	virtual void run() = 0;

	std::string& getSqlQuery() {
		return this->m_sql_query;
	}

protected:
	std::string	m_sql_query;

};

} /* namespace DB */
} /* namespace SF */
#endif /* WORKITEMQUERYBASE_H_ */
