/*
 * CTestPing.cc
 *
 *  Created on: 11.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CTestPing.h"

#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <string>

#include <softel/core/net/CPing.h>
#include <softel/core/net/CPingCmtsCm.h>

using namespace std;
using namespace SF::CORE::NET;

namespace SF {
namespace OSS {
namespace EXEC {

CTestPing::CTestPing() {
	// TODO Auto-generated constructor stub

}

CTestPing::~CTestPing() {
	// TODO Auto-generated destructor stub
}

void CTestPing::run() {

	//CPing p("dir.bg");
	CPing p("softel.bg");

	p.run();
	p.dumpInfo();

	CPingCmtsCm p2("softel.bg");

	p2.run();
	p2.dumpInfo();

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

