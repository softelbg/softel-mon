/*
 * CMonNetElemUps.h
 *
 *  Created on: 25.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CMONNETELEMUPS_H_
#define CMONNETELEMUPS_H_

#include "BMonNetElemBase.h"
#include "CSnmpOidListUps.h"

namespace SF {
namespace OSS {
namespace NMS {

class CMonNetElemUps: public BMonNetElemBase {
public:
	CMonNetElemUps();
	virtual ~CMonNetElemUps();

	virtual void run();
	virtual void init();

private:

	CSnmpOidListUps m_snmp_oid_list;

};

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CMONNETELEMUPS_H_ */
