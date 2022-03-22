/*
 * CTestFileIni.cc
 *
 *  Created on: 29.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CTestFileIni.h"
#include <softel/core/util/BFileIniBase.h>

using namespace std;
using namespace SF::CORE::UTIL;

namespace SF {
namespace OSS {
namespace EXEC {

CTestFileIni::CTestFileIni() {
	// TODO Auto-generated constructor stub

}

CTestFileIni::~CTestFileIni() {
	// TODO Auto-generated destructor stub
}

void CTestFileIni::run() {

	BFileIniBase iniFile("/etc/CMTSMon.ini");
	iniFile.dumpInfo();

	cout << __FILE__ << " MySQL::User => " << iniFile.get("MySQL", "User") << endl;

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

