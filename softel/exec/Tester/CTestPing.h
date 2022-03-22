/*
 * CTestPing.h
 *
 *  Created on: 11.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTESTPING_H_
#define CTESTPING_H_

#include "CTestCase.h"

namespace SF {
namespace OSS {
namespace EXEC {

class CTestPing : public CTestCase {
public:
	CTestPing();
	virtual ~CTestPing();

	virtual void run();

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTESTPING_H_ */
