/*
 * CMTSMon.cc
 *
 *  Created on: 03.10.2011
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include <vector>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <softel/core/error/CErrorCommon.h>
#include <softel/core/error/CErrorSql.h>
#include <softel/oss/cmts/CCmtsList.h>
#include <softel/oss/cmts/CCmtsLog.h>
#include <softel/core/system/CSystemTools.h>

using namespace SF::OSS::CMTS;
using namespace SF::CORE::ERROR;

int main(int argc, char *argv[]) {

	SF::CORE::SYSTEM::CSystemTools::doInitialFork();

	try {

		// Command line parameters
		if (argc > 1) {

		};

		LOG(LOG_LEVEL_DEBUG, "CMTSMon Started...");

		CCmtsList cmtsList;

		cmtsList.launchCmts();

	} catch (CErrorSql& error) {
		LOG(LOG_LEVEL_ERROR, error.getMessage());
	} catch (CErrorCommon& error) {
		LOG(LOG_LEVEL_ERROR, error.getMessage());
	};

}

