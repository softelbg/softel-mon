/*
 * CTestLauncher.cc
 *
 *  Created on: 13.03.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CTestLauncher.h"

#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <string>

#include "CTestCase.h"
#include "CTestPing.h"
#include "CTestThrPool.h"
#include "CTestThrPoolAsio.h"
#include "CTestExecuteToolsAndTasks.h"
#include "CTestStringUtil.h"
#include "CTestStatistics.h"
#include "CTestFileIni.h"


using namespace std;

namespace SF {
namespace OSS {
namespace EXEC {

CTestLauncher::CTestLauncher() {

}

CTestLauncher::~CTestLauncher() {

}

/**
 * Launch all Tests.
 *
 * TODO: Should do some command line/file conf params to define Test Case List.
 *
 */
void CTestLauncher::run() {

	vector<CTestCase*> listTestCases;
	
	//listTestCases.push_back(new CTestPing);
	//listTestCases.push_back(new CTestThrPool);
	//listTestCases.push_back(new CTestStringUtil);
	//listTestCases.push_back(new CTestStatistics);
	listTestCases.push_back(new CTestFileIni);
	//listTestCases.push_back(new CTestThrPoolAsio);

	//listTestCases.push_back(new CTestExecuteToolsAndTasks);

	for(size_t i = 0; i < listTestCases.size(); i++) {
		listTestCases[i]->run();
	}

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */
