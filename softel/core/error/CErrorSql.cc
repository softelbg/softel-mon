/*
 * CErrorSql.cc
 *
 *  Created on: 14.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CErrorSql.h"



namespace SF {
namespace CORE {
namespace ERROR {

CErrorSql::CErrorSql(const std::string& message, const std::string& sql, int errorn) :
	BErrorBase(message), m_sql(sql), m_errorn(errorn) {

	std::stringstream ss;

	ss << __FUNCTION__ << ":" << errorn << "::" << message << "::" << sql;

	this->setMessage(ss.str());

}

CErrorSql::CErrorSql(const std::string& message) :
	BErrorBase(message) {

	std::stringstream ss;

	ss << __FUNCTION__ << ":" << message;

	this->setMessage(ss.str());

}

CErrorSql::CErrorSql(const std::string& message, int errorn) :
			BErrorBase(message), m_errorn(errorn) {

	std::stringstream ss;

	ss << __FUNCTION__ << ":" << errorn << "::" << message;

	this->setMessage(ss.str());

}

CErrorSql::~CErrorSql() {
	// TODO Auto-generated destructor stub
}

} /* namespace ERROR */
} /* namespace CORE */
} /* namespace SF */

