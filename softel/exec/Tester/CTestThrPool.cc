/*
 * CTestThrPool.cc
 *
 *  Created on: 12.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CTestThrPool.h"

#include "CTestJob.h"
#include <softel/core/util/CThreadPoolBase.h>
#include <softel/core/system/CSystemTools.h>

using namespace std;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace OSS {
namespace EXEC {

CTestThrPool::CTestThrPool() {
	// TODO Auto-generated constructor stub

}

CTestThrPool::~CTestThrPool() {
	// TODO Auto-generated destructor stub
}

void CTestThrPool::run() {

	CThreadPoolBase pool(20);

	for(int i = 1; i < 100; i++) {

		int sleep = i;

		CTestJob* job = new CTestJob(sleep);

		job->setShouldDelete(true);

		//pool.get()->setJob(&job);

		pool.addJob(job);
	}

	while(pool.getJobQueueSize() > 0) {

		pool.processQueue();

		cout << __FILE__ << ": waiting the pool jobs reamining: " << pool.getJobQueueSize() << endl;

		CSystemTools::doSleep(3);
	}

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */


