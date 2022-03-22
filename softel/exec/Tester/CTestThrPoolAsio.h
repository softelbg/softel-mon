/*
 * CTestThrPoolAsio.h
 *
 *  Created on: 18.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTESTTHRPOOLASIO_H_
#define CTESTTHRPOOLASIO_H_

#include "CTestCase.h"

namespace SF {
namespace OSS {
namespace EXEC {

/*
 *
 */
class CTestThrPoolAsio: public SF::OSS::EXEC::CTestCase {
public:
	CTestThrPoolAsio();
	virtual ~CTestThrPoolAsio();

	virtual void run();

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */
#endif /* CTESTTHRPOOLASIO_H_ */
