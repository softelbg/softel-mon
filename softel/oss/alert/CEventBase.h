/*
 * CEventBase.h
 *
 *	Base Event. Should be subclassed by different Event types.
 *	Events should be analyzed to make a CEP to raise an Alert.
 *	Alerts are special kind of Event - single Alert can have many Events which caused it.
 *
 *
 *  Created on: 13.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CEVENTBASE_H_
#define CEVENTBASE_H_

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

#include "CSqlAlertTools.h"
#include "CommonAlert.h"

#include <softel/db/CMySqlDB.h>
#include <softel/oss/cmts/SqlTableNames.h>
#include <softel/db/CDataRow.h>
#include <softel/core/system/CSystemTools.h>
#include <softel/db/CParamSet.h>

namespace SF {
namespace OSS {
namespace ALERT {

class CEventBase {
public:
	CEventBase();
	virtual ~CEventBase();

	virtual void init();

	void setParam(const std::string& key, const std::string& val);
	std::string getParam(const std::string& key);

	void setMessage(const std::string& message);

	virtual void dumpInfo();

protected:

	SF::DB::CParamSet	m_param_set;

	SF::DB::CMySqlDB* m_pDb;

	CSqlAlertTools m_dbTools;

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CEVENTBASE_H_ */
