/*
 * CFork.h
 *
 * Fork representation...
 *
 *  Created on: 20.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CFORK_H_
#define CFORK_H_

#include "CSystemTools.h"
#include <softel/core/error/CErrorCommon.h>

namespace SF {
namespace CORE {
namespace SYSTEM {

class CFork {

public:
	CFork();

	int doInitialFork(); // Do a fork and exit with main process
	int doFork();

	bool isParent();
	bool isChild();

	int getChildPid();
	int getSelfPid();

private:
	int pidChild;
	int pidSelf;

};

} /* namespace SYSTEM */
} /* namespace CORE */
} /* namespace SF */


#endif /* CFORK_H_ */
