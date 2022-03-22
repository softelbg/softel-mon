/*
 * CTestCase.h
 *
 *  Created on: 11.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTESTCASE_H_
#define CTESTCASE_H_

namespace SF {
namespace OSS {
namespace EXEC {

class CTestCase {
public:
	CTestCase();
	virtual ~CTestCase();

	virtual void run()=0;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTESTCASE_H_ */
