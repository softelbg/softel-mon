/*
 * SnmpTrap.h
 *
 *  Created on: 28.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef SNMPTRAP_H_
#define SNMPTRAP_H_

#include <softel/core/CObject.h>
#include "SnmpSession.h"
#include "SnmpPackageBuilder.h"
#include "SnmpPdu.h"

namespace SF {
namespace CORE {
namespace NET {

/*
 *
 */
class SnmpTrap: public SF::CORE::CObject {
public:
	SnmpTrap();
	virtual ~SnmpTrap();

	/*
	 * Send SNMP trap.
	 */
	bool SendTrap(const SnmpSession& session, const SnmpPdu& pdu, int trapType);

	/*
	 * SNMP specific trap functions.
	 */
	bool SendTestTrap();
	bool SendColdStartTrap();
	bool SendSpecialTrap(int specialTrapType);

private:
	bool BuildPackage(SnmpPackageBuilder& builder, const SnmpSession& session,
			const SnmpPdu& pdu, int trapType);
	bool SendPackage(const SnmpPackageBuilder& builder,
			const SnmpSession& session);
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* SNMPTRAP_H_ */
