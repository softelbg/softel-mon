/*
 * CErrorSql.h
 *
 *	SQL Error. Should handle error message, error code and the SQL caused the error.
 *
 *  Created on: 14.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CERRORSQL_H_
#define CERRORSQL_H_

#include <string>
#include <sstream>

#include "BErrorBase.h"


namespace SF {
namespace CORE {
namespace ERROR {


class CErrorSql: public BErrorBase {
public:
	CErrorSql(const std::string& message);
	CErrorSql(const std::string& message, int errorn);
	CErrorSql(const std::string& message, const std::string& sql, int errorn);
	virtual ~CErrorSql();

	int getErrorn() const {
		return m_errorn;
	}

	std::string getSql() const {
		return m_sql;
	}

private:
	std::string	m_sql;
	int		m_errorn;
};

} /* namespace ERROR */
} /* namespace CORE */
} /* namespace SF */

#endif /* CERRORSQL_H_ */
