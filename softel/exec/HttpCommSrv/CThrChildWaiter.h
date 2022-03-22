/*
 * CThrChildWaiter.h
 *
 *  Created on: 07.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTHRCHILDWAITER_H_
#define CTHRCHILDWAITER_H_

#include <softel/core/util/CThreads.h>

namespace SF {
namespace OSS {
namespace EXEC {

class CThrChildWaiter: public SF::CORE::UTIL::CThreads {
public:
	CThrChildWaiter();
	virtual ~CThrChildWaiter();

protected:

	int run();

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTHRCHILDWAITER_H_ */
