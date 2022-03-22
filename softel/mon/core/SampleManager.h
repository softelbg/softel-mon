/*
 * SampleManager.h
 *
 * Sample Manager.
 * Should manage Sample I/O...
 *
 *  Created on: 18.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef SAMPLEMANAGER_H_
#define SAMPLEMANAGER_H_

#include <softel/core/ManagerBase.h>

namespace SF {
namespace MON {
namespace CORE {

/*
 *
 */
class SampleManager: public SF::CORE::ManagerBase {
public:
	SampleManager();
	virtual ~SampleManager();
};

} /* namespace CORE */
} /* namespace MON */
} /* namespace SF */
#endif /* SAMPLEMANAGER_H_ */
