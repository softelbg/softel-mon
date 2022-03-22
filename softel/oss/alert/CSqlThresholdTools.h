/*
 * CSqlThresholdTools.h
 *
 *  Created on: 02.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSQLTHRESHOLDTOOLS_H_
#define CSQLTHRESHOLDTOOLS_H_

#include <softel/oss/cmts/CSqlToolsBase.h>
#include <softel/oss/cmts/CSnmpOid.h>
#include <softel/oss/cmts/SqlTableNames.h>
#include <softel/db/CDataSet.h>

namespace SF {
namespace OSS {
namespace ALERT {

class CSqlThresholdTools: public SF::OSS::CMTS::CSqlToolsBase {
public:
	CSqlThresholdTools();
	virtual ~CSqlThresholdTools();

	void sqlSelectThresholdList(const std::string& filter, SF::DB::CDataSet& ds);
	void sqlSelectThresholdListCm(SF::DB::CDataSet& ds);
	void sqlSelectThresholdListIf(SF::DB::CDataSet& ds);

	void sqlSelectThresholdByOid(const SF::OSS::CMTS::CSnmpOid& oid, SF::DB::CDataSet& ds);

	bool selectOidListFiltered(const std::string& filter, std::vector<SF::OSS::CMTS::CSnmpOid>& oidList);
	bool selectOidList(std::vector<SF::OSS::CMTS::CSnmpOid>& oidList);

	void selectCmLatest(SF::DB::CDataSet& ds);
	void selectCmLatest(const std::string& filter, SF::DB::CDataSet& ds);
};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CSQLTHRESHOLDTOOLS_H_ */
