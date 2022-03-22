/*
 * CTestExecuteToolsAndTasks.h
 *
 *  Created on: 06.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTESTEXECUTETOOLSANDTASKS_H_
#define CTESTEXECUTETOOLSANDTASKS_H_

#include "CTestCase.h"

namespace SF {
namespace OSS {
namespace EXEC {

class CTestExecuteToolsAndTasks: public CTestCase {
public:
	CTestExecuteToolsAndTasks();
	virtual ~CTestExecuteToolsAndTasks();

	void run();

private:

	void doDeleteDuplicateListCm();
	void doDropMemoryTables();
	void doZeroWrongParams();
	void doAlterTablesIf();
	void doDropBigTables();

	void doDedicatedCmList();
	void doDedicateCmListCoordinatesUpdate();

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTESTEXECUTETOOLSANDTASKS_H_ */
