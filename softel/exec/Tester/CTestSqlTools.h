/*
 * CTestSqlTools.h
 *
 *  Created on: 06.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTESTSQLTOOLS_H_
#define CTESTSQLTOOLS_H_

#include <softel/oss/cmts/CSqlToolsBase.h>
#include <softel/db/CDataSet.h>

#include <string>
#include <map>
#include <vector>

namespace SF {
namespace OSS {
namespace EXEC {

class CTestSqlTools: public SF::OSS::CMTS::CSqlToolsBase {
public:
	CTestSqlTools();
	CTestSqlTools(int numconn);
	virtual ~CTestSqlTools();

	void deleteDuplicateListCm();

	void zeroWrongParams(const std::string& tablePrefix, const std::string& tableDate);

	void dropMemoryTables();

	void alterTablesAddColumn(const std::string& tablePrefix, const std::string& tableDate, const std::string& colPart);

	void dropBigTables(const std::string& tablePrefix, const std::string& tableDate);

	void selectListCmtsIf(const std::string& cmts_id, const std::string& if_type, SF::DB::CDataSet& ds);
	void selectListCmtsIfUp(const std::string& cmts_id, SF::DB::CDataSet& ds);

	void selectFnDataCmtsIf(const std::string& cmts_id, const std::string& if_id, const std::vector<std::string>& listTables, SF::DB::CDataSet& ds);

private:

	void doWrongParamSet(std::map<std::string, int>& params);

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTESTSQLTOOLS_H_ */
