/*
 * CCmtsCmLatestRaw.h
 *
 *  Created on: 04.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCMTSCMLATESTRAW_H_
#define CCMTSCMLATESTRAW_H_

#include "CSqlThresholdTools.h"
#include "CThresholdOidList.h"

#include <softel/db/CDataSet.h>

namespace SF {
namespace OSS {
namespace ALERT {

class CCmtsCmLatestRaw {
public:
	CCmtsCmLatestRaw();
	virtual ~CCmtsCmLatestRaw();

	void init();

	void compare(CThresholdOidList& listOidThresholds);

	const SF::DB::CDataSet& getRawDataSet() const;
	const std::map<std::string, std::vector<CThresholdResult*> >& getRawCmThresholdResults();
	const std::vector<CThresholdResult*>& getRawCmThresholdResults(const std::string& severity);

	void dumpInfo() const;

private:
	SF::DB::CDataSet	m_data_set;

	std::map<std::string, std::vector<CThresholdResult*> > m_list_compared_raw_cm_thresults;

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CCMTSCMLATESTRAW_H_ */
