/*
 * CTestExecuteToolsAndTasks.cc
 *
 *  Created on: 06.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CTestExecuteToolsAndTasks.h"
#include "CTestSqlTools.h"
#include "CDedicatedCmList.h"
#include "CDedicatedCmGpsCoordinatesUpdate.h"

using namespace std;

namespace SF {
namespace OSS {
namespace EXEC {

CTestExecuteToolsAndTasks::CTestExecuteToolsAndTasks() {
	// TODO Auto-generated constructor stub

}

CTestExecuteToolsAndTasks::~CTestExecuteToolsAndTasks() {
	// TODO Auto-generated destructor stub
}

/**
 * Run here some tasks and tools.
 * Free to use...
 */
void CTestExecuteToolsAndTasks::run() {

	//doDedicateCmListCoordinatesUpdate();
	//doAlterTablesIf();

	doDropBigTables();

}

/**
 * Delete Duplicate CMs from List Memory Table.
 */
void CTestExecuteToolsAndTasks::doDeleteDuplicateListCm() {

	CTestSqlTools sqlTools;

	sqlTools.deleteDuplicateListCm();

}

/**
 * Drop all memory tables.
 * Needed to allow setting max heap size when created.
 */
void CTestExecuteToolsAndTasks::doDropMemoryTables() {

	CTestSqlTools sqlTools;

	sqlTools.dropMemoryTables();

}

/**
 * Set to zero some wrong saved params.
 * Error corrected/uncorrected could be wrong when the error list is empty
 * (if db server restarted). Should be fixed in SnmpValue calculation.
 *
 */
void CTestExecuteToolsAndTasks::doZeroWrongParams() {

	CTestSqlTools sqlTools;

	sqlTools.zeroWrongParams("oss_cm_", "201210");
	sqlTools.zeroWrongParams("oss_if_", "201210");

}

/**
 * Read Dedicated CM List from file and fill into DB.
 *
 */
void CTestExecuteToolsAndTasks::doDedicatedCmList() {

	CDedicatedCmList cmList("cmlist.txt");

	cmList.executeSqlList();

}

void CTestExecuteToolsAndTasks::doAlterTablesIf() {

	CTestSqlTools sqlTools;

	sqlTools.alterTablesAddColumn("oss_if_", "201301", " up_avg_util int DEFAULT 0 ");
	sqlTools.alterTablesAddColumn("oss_if_", "", " up_avg_util int DEFAULT 0 ");
}

void CTestExecuteToolsAndTasks::doDropBigTables() {

	CTestSqlTools sqlTools(3);

	sqlTools.dropBigTables("oss_cm_", "201301");
}

/**
 * Read Dedicated CM List with GPS Coordinates and update already existing CM List into DB.
 *
 */
void CTestExecuteToolsAndTasks::doDedicateCmListCoordinatesUpdate() {

	vector<CDedicatedCmGpsCoordinatesUpdate*> listCmLists;

	listCmLists.push_back(new CDedicatedCmGpsCoordinatesUpdate("cablemodem_monitoring_center.csv"));
	listCmLists.push_back(new CDedicatedCmGpsCoordinatesUpdate("GPS_coordinates_Sever_CM.csv"));

	for(size_t i = 0; i < listCmLists.size(); i++) {

		listCmLists[i]->dumpInfo();

		listCmLists[i]->executeSql();
	}

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */




