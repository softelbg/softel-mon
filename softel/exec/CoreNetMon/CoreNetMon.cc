/*
 * CoreNetMon.cc
 *
 *  Created on: 25.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include "CSchedulerMainCoreNetMon.h"

#include <softel/core/error/CErrorCommon.h>
#include <softel/core/error/CErrorSql.h>
#include <softel/core/system/CSystemTools.h>

using namespace std;
using namespace SF::OSS::EXEC;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::SYSTEM;

int main(int argc, char *argv[]) {

	SF::CORE::SYSTEM::CSystemTools::doInitialFork();

	try {

		CSchedulerMainCoreNetMon schedulerCoreNetMon;

		schedulerCoreNetMon.setPeriodSleep(30);

		schedulerCoreNetMon.start();


	} catch (CErrorSql& error) {
		//CLog Log("oss."__FILE__".log", error.getMessage());
		cout << "oss." << __FILE__ << ".log" << error.getMessage();
	} catch (CErrorCommon& error) {
		//CLog Log("oss."__FILE__".log", error.getMessage());
		cout << "oss." << __FILE__ << ".log" << error.getMessage();
	}

}

