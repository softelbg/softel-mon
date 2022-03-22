/*
 * CSqlCmAlert.cc
 *
 *	CMTS CM Alert SQL Handling component
 *
 *  Created on: 22.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSchedulerCmAlert.h"
#include <softel/core/error/CErrorCommon.h>
#include <softel/core/error/CErrorSql.h>
#include <softel/oss/cmts/CCmtsAlertLog.h>
#include <softel/core/system/CSystemTools.h>

using namespace std;
using namespace SF::OSS::CMTS;
using namespace SF::OSS::EXEC;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::SYSTEM;

int main(int argc, char *argv[]) {

	SF::CORE::SYSTEM::CSystemTools::doInitialFork();

	try {

		LOGA(LOG_LEVEL_INFO, string(__FILE__) + " Started...");

		CSchedulerCmAlert cmScheduler;

		cmScheduler.start();


	} catch (CErrorSql& error) {
		LOGA(LOG_LEVEL_ERROR, error.getMessage());
	} catch (CErrorCommon& error) {
		LOGA(LOG_LEVEL_ERROR, error.getMessage());
	}

}
