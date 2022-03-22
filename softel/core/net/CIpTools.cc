/*
 * CIpTools.cc
 *
 *  Created on: 10.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CIpTools.h"
#include "CIp.h"

namespace SF {
namespace CORE {
namespace NET {

CIpTools::CIpTools() {

}

CIpTools::~CIpTools() {

}

/**
 * Reset Host using oid 1.3.6.1.2.1.69.1.1.3.0
 * example command: snmpset -O n -v 2c -c modemsrw $ip .1.3.6.1.2.1.69.1.1.3.0 i 1
 *
 */
bool CIpTools::snmpResetHost(const std::string& ip) {

	CIp host(ip);
	char snmpResult[256];
	char rwcomm[32] = "modemsrw";
	char snmpSetValue[16] = "1";
	char snmpOid[64] = ".1.3.6.1.2.1.69.1.1.3.0";
	char snmpType = 'i';

	bool result;

	host.SnmpSet(snmpOid, rwcomm, snmpType, snmpSetValue, snmpResult);

	// Make result from snmp result...
	if (snmpResult[0] == '1') {
		result = true;
	} else {
		result = false;
	}

	//cout << __FILE__ << ":" << __FUNCTION__ << ": snmpResult:" << result << endl;

	return result;
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

