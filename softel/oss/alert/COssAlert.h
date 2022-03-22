/*
 * COssAlert.h
 *
 *	Base OSS Alert. Should be a common representation of an Alert.
 *	Alert is a Event, but can own/caused by multiple other Events.
 *	Alert is a major Event which should be considered as a CEP and should be responded by user.
 *
 *  Created on: 24.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef COSSALERT_H_
#define COSSALERT_H_

#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "CSqlAlertTools.h"
#include "CommonAlert.h"
#include "CEventBase.h"

#include <softel/db/CMySqlDB.h>
#include <softel/oss/cmts/SqlTableNames.h>
#include <softel/db/CDataRow.h>
#include <softel/core/system/CSystemTools.h>
#include <softel/db/CParamSet.h>

namespace SF {
namespace OSS {
namespace ALERT {

class COssAlert : public CEventBase {

public:
	COssAlert();
	COssAlert(SF::DB::CMySqlDB* pDb);
	COssAlert(SF::DB::CDataRow& row);
	virtual ~COssAlert();

	void setAlertType(int type);
	void setAlertStatus(int status);
	void setAlertStatusClosed();
	void setAlertSeverity(int severity);
	void setAlertCounter(int count);
	void setAlertDateInit(long uxdate);

	void setAlertMessage(const std::string& message);

	std::string getAlertType();
	std::string getAlertStatus();
	std::string getAlertSeverity();
	std::string getAlertCounter();
	std::string getAlertDateInit();

	virtual void initAlertFromDb(); // Load from DB alert info
	virtual void initAlert(SF::DB::CDataRow& row);
	virtual void initAlert();

	virtual void saveAlert();
	virtual void doNewAlert();
	virtual void doCloseAlert();
	virtual void updateAlert();

protected:


private:

	void initAlertStatus(); // Set Open status
	void initAlertCounter(); // set the counter = 1
	void initAlertSeverity(); // set severity to most urgent = 0
	void initDate(); // set init date
	void initDateCurrent(); // set current date datestamp


};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */



#endif /* COSSALERT_H_ */
