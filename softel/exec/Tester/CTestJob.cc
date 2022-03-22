/*
 * CTestJob.cc
 *
 *  Created on: 12.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CTestJob.h"

#include <softel/core/system/CSystemTools.h>

#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace OSS {
namespace EXEC {

CTestJob::CTestJob(int sleep) {
	// TODO Auto-generated constructor stub

	m_sleep = sleep;
}

CTestJob::~CTestJob() {
	// TODO Auto-generated destructor stub
}

void CTestJob::run() {

	cout << __FILE__ << ": sleep: " << m_sleep << " Started" << endl;

	CSystemTools::doSleep(m_sleep);

	cout << __FILE__ << ": sleep: " << m_sleep << " Ended" << endl;

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

