/*
 * CTestStatistics.h
 *
 *  Created on: 06.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTESTSTATISTICS_H_
#define CTESTSTATISTICS_H_

#include <softel/db/CDataSet.h>
#include "CTestCase.h"

namespace SF {
namespace OSS {
namespace EXEC {

class CTestStatistics : public CTestCase {
public:
	CTestStatistics();
	virtual ~CTestStatistics();

	virtual void run();

	void statPeriodLowSnrCmtsIfUp(const std::string& cmts_id);

private:

	void doStatPeriodCmtsIf(const std::string& cmts_id,
			const std::string& if_id,
			SF::DB::CDataSet ds);

	void doStatPeriodCmtsIf(const std::string& cmts_id,
			const std::string& if_id,
			const std::string& col_key,
			const std::string& thrDwn,
			const std::string& thrUp,
			SF::DB::CDataSet ds);

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTESTSTATISTICS_H_ */
