/*
 * CSqlCmAlert.h
 *
 *	CMTS CM Alert SQL Handling component
 *
 *  Created on: 22.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSQLCMALERT_H_
#define CSQLCMALERT_H_

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <unistd.h>

#include <softel/db/CDataSet.h>
#include <softel/db/CDataRow.h>
#include <softel/oss/cmts/CMySqlConn.h>
#include <softel/oss/cmts/SqlTableNames.h>
#include <softel/oss/cmts/SnmpOids.h>
#include <softel/oss/cmts/CCmtsToolsSql.h>
#include <softel/oss/alert/CommonAlert.h>

namespace SF {
namespace OSS {
namespace EXEC {

class CSqlCmAlert {

public:

	static CSqlCmAlert* getInstance();

	bool sqlGetCmList(SF::DB::CDataSet& cmList);
	bool sqlGetCmList(SF::DB::CMySqlDB* pDb, SF::DB::CDataSet& cmList);

	bool sqlGetCmAlertList(SF::DB::CDataSet& dsCmAlert);
	bool sqlGetCmAlertList(SF::DB::CMySqlDB* pDb, SF::DB::CDataSet& dsCmAlert);

	bool doCmtsCmOidList(const std::string& cmtsId, std::vector<SF::OSS::CMTS::CSnmpOid>& listOidCm);
	bool doCmtsCmOidList(SF::DB::CMySqlDB* pDb, const std::string& cmtsId, std::vector<SF::OSS::CMTS::CSnmpOid>& listOidCm);

private:
	CSqlCmAlert();

	static CSqlCmAlert* m_pInstance;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */


#endif /* CSQLCMALERT_H_ */
