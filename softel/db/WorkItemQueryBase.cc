/*
 * WorkItemQueryBase.cc
 *
 *  Created on: 27.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "WorkItemQueryBase.h"

namespace SF {
namespace DB {

WorkItemQueryBase::WorkItemQueryBase(const std::string& sqlQuery) :
	m_sql_query(sqlQuery) {

	this->setParam("SQL", sqlQuery);

}

WorkItemQueryBase::~WorkItemQueryBase() {
	// TODO Auto-generated destructor stub
}

} /* namespace DB */
} /* namespace SF */
