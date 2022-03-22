/*
 * CSqlAlertTools.h
 *
 * SQL tools handling Alerts
 *
 *  Created on: 01.08.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSQLALERTTOOLS_H_
#define CSQLALERTTOOLS_H_

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

#include "CommonAlert.h"

#include <softel/oss/cmts/CMySqlConn.h>
#include <softel/oss/cmts/CSqlToolsBase.h>
#include <softel/oss/cmts/SnmpOids.h>
#include <softel/db/CParamSet.h>
#include <softel/db/CDataSet.h>


namespace SF {
namespace OSS {
namespace ALERT {

class CSqlAlertTools : public SF::OSS::CMTS::CSqlToolsBase {

public:
	CSqlAlertTools();
	~CSqlAlertTools();

	void setAlert(SF::DB::CParamSet* pParamSet);

	void sqlUpdateAlertBase(); // SQL UPDATE Base Alert accoding to current alert params
	void sqlInsertAlertBase(); // SQL Insert Base Alert
	void sqlIncAlertBase(); // Increment Counter of Base Alert

	void sqlInsertAlertCm();
	void sqlInsertAlertIf();

	void sqlSelectAlertCmDedicated(SF::DB::CDataSet& ds);
	void sqlSelectAlertCmtsIfCounter(SF::DB::CDataSet& ds);

	void sqlSelectAlertCmtsIfCounterList(SF::DB::CDataSet& ds);

	void sqlSelectCmIfDocs3List(SF::DB::CDataSet& ds);

private:

	SF::DB::CParamSet*	m_pParamSet;

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */


#endif /* CSQLALERTTOOLS_H_ */
