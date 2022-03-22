/*
 * CTestThrPool.h
 *
 *  Created on: 12.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTESTTHRPOOL_H_
#define CTESTTHRPOOL_H_

#include "CTestCase.h"

namespace SF {
namespace OSS {
namespace EXEC {

class CTestThrPool : public CTestCase {
public:
	CTestThrPool();
	virtual ~CTestThrPool();

	virtual void run();

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTESTTHRPOOL_H_ */
