/*
 * CTestJob.h
 *
 *  Created on: 12.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTESTJOB_H_
#define CTESTJOB_H_

#include <softel/core/util/CWorkerJob.h>

namespace SF {
namespace OSS {
namespace EXEC {

class CTestJob : public SF::CORE::UTIL::CWorkerJob {
public:
	CTestJob(int sleep);
	virtual ~CTestJob();

	virtual void run();

private:
	int	m_sleep;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTESTJOB_H_ */
