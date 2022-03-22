/*
 * BSingletonBase.h
 *
 *  Created on: 29.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BSINGLETONBASE_H_
#define BSINGLETONBASE_H_

#include <stdio.h>
#include <string>

namespace SF {
namespace CORE {
namespace UTIL {

class BSingletonBase {
public:
	virtual ~BSingletonBase();

	static BSingletonBase* getInstance();

private:
	BSingletonBase();

	static BSingletonBase* m_pInstance;

};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

#endif /* BSINGLETONBASE_H_ */
