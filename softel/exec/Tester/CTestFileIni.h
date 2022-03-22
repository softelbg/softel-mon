/*
 * CTestFileIni.h
 *
 *  Created on: 29.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTESTFILEINI_H_
#define CTESTFILEINI_H_

#include "CTestCase.h"

namespace SF {
namespace OSS {
namespace EXEC {

class CTestFileIni: public CTestCase {
public:
	CTestFileIni();
	virtual ~CTestFileIni();

	virtual void run();

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTESTFILEINI_H_ */
