/*
 * WorkItemMySqlQuery.h
 *
 *  Created on: 27.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef WORKITEMMYSQLQUERY_H_
#define WORKITEMMYSQLQUERY_H_

#include "WorkItemQueryBase.h"

namespace SF {
namespace DB {

/*
 *
 */
class WorkItemMySqlQuery: public SF::DB::WorkItemQueryBase {
public:
	WorkItemMySqlQuery(const std::string& sqlQuery);
	virtual ~WorkItemMySqlQuery();

	virtual void run();

};

} /* namespace DB */
} /* namespace SF */
#endif /* WORKITEMMYSQLQUERY_H_ */
