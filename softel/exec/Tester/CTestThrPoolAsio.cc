/*
 * CTestThrPoolAsio.cc
 *
 *  Created on: 18.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CTestThrPoolAsio.h"

#include <softel/core/commonDefs.h>
#include <softel/core/pool/WorkItemBase.h>
#include <softel/core/pool/WorkItemMokup.h>
#include <softel/core/system/CSystemTools.h>

using namespace std;
using namespace SF::CORE::POOL;

namespace SF {
namespace OSS {
namespace EXEC {

CTestThrPoolAsio::CTestThrPoolAsio() {
	// TODO Auto-generated constructor stub

}

CTestThrPoolAsio::~CTestThrPoolAsio() {
	// TODO Auto-generated destructor stub
}

void CTestThrPoolAsio::run() {

	ThreadPoolAsio thPool(10);

	for (int j = 0; j < 10; j++) {
		for (int i = 0; i < 10; i++) {
			SF::CORE::Ptr<WorkItemBase> ptrWi(new WorkItemMokup(i));
			ptrWi->setParam("id", i);
			ptrWi->setParam("desc", "Test WorkItem");

			thPool.postWorkItem(ptrWi);
		}

		thPool.printStatus();

		SF::CORE::SYSTEM::CSystemTools::doSleep(10);
	}

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */
