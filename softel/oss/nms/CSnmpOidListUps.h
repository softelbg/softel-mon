/*
 * CSnmpOidListUps.h
 *
 *  Created on: 04.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSNMPOIDLISTUPS_H_
#define CSNMPOIDLISTUPS_H_

#include "BSnmpOidListBase.h"

namespace SF {
namespace OSS {
namespace NMS {

class CSnmpOidListUps: public BSnmpOidListBase {
public:
	CSnmpOidListUps();
	virtual ~CSnmpOidListUps();

	virtual void init();

};

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CSNMPOIDLISTUPS_H_ */
