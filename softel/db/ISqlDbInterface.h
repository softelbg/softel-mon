/*
 * ISqlDbInterface.h
 *
 *	Abstract Sql Db
 *
 *  Created on: 14.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef ISQLDBINTERFACE_H_
#define ISQLDBINTERFACE_H_

#include <string>

#include "CDataSet.h"

namespace SF {
namespace DB {

class ISqlDbInterface {
public:
	virtual ~ISqlDbInterface();

	virtual bool doSelect(const std::string& sqlQuery, CDataSet& ds)=0;
	virtual bool doExecute(const std::string& sqlQuery)=0;
	virtual long doInsert(const std::string& sqlQuery)=0;
	virtual long doUpdate(const std::string& sqlQuery)=0;

};

} /* namespace DB */
} /* namespace SF */

#endif /* ISQLDBINTERFACE_H_ */
