/*
 * SnmpTrap.cc
 *
 *  Created on: 28.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "SnmpTrap.h"
#include "SnmpSession.h"

#include <unistd.h>
#include <netdb.h>

namespace SF {
namespace CORE {
namespace NET {

SnmpTrap::SnmpTrap() {
	// TODO Auto-generated constructor stub

}

SnmpTrap::~SnmpTrap() {
	// TODO Auto-generated destructor stub
}

bool SnmpTrap::SendTrap(const SnmpSession& session, const SnmpPdu& pdu,
		int trapType) {

	SnmpPackageBuilder builder;
	if (!BuildPackage(builder, session, pdu, trapType)) {
		return false;
	}

	return SendPackage(builder, session);
}

bool SnmpTrap::SendTestTrap() {
	return SendSpecialTrap(SNMP_TEST_TRAP);
}

bool SnmpTrap::SendColdStartTrap() {
	return SendSpecialTrap(SNMP_COLDSTART_TRAP);
}

/*
 *----------------------------------------------------------------------
 *
 * SnmpTrap::BuildPackage --
 *
 *    Build SNMP trap package.
 *
 *    Package is build as
 *       [version] [community] [PDU type(4)] [enterprise] [agent address]
 *       [trap type(6)] [specific code] [time stamp]
 *       [var1 name] [var1 data] [var2 name] [var2 data] ...
 *
 *    NOTES:
 *       To make the package building efficient, the package is built
 *       from rear to front.
 *
 * Results:
 *    True on success, false on error.
 *
 *----------------------------------------------------------------------
 */
bool SnmpTrap::BuildPackage(SnmpPackageBuilder& builder,
		const SnmpSession& session, const SnmpPdu& pdu, int trapType) {

	if (session.m_version != 1) {
		return false;
	} else {
		// Build pdu.
		if (!pdu.BuildPackage(builder)) {
			return false;
		}

		int snmpTrapType;
		if (trapType != SNMP_COLDSTART_TRAP) {
			snmpTrapType = ASN_TRAP_GENERIC_ENT_TYPE;
		} else {
			snmpTrapType = ASN_TRAP_GENERIC_CS_TYPE; // Cold start traps
		}

		// Build trap header
		if (!builder.PushUnLong(ASN_TYPE_TIME, session.m_timestamp)
				|| !builder.PushLong(ASN_TYPE_INTEGER, session.m_traptype)
				|| !builder.PushLong(ASN_TYPE_INTEGER, snmpTrapType)
				|| !builder.PushIP(ASN_TYPE_IP, session.m_agent)
				|| !builder.PushOID(ASN_TYPE_OID, session.m_enterprise)) {
			return false;
		}
		int trapheader = builder.GetSize();
		if (!builder.PushSequence(ASN_TYPE_TRAP, trapheader)) {
			return false;
		}

		// Build snmp header
		if (!builder.PushString(ASN_TYPE_STRING, session.m_community)
				|| !builder.PushLong(ASN_TYPE_INTEGER, session.m_version - 1)) {
			return false;
		}
		int snmpheader = builder.GetSize();
		if (!builder.PushSequence(ASN_TYPE_SEQUENCE, snmpheader)) {
			return false;
		}
	}

	return true;
}

bool SnmpTrap::SendPackage(const SnmpPackageBuilder& builder,
		const SnmpSession& session) {

	struct hostent *h = gethostbyname(session.m_destination.c_str());
	if (h == NULL) {
		return false;
	}

	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if (!s) {
		return false;
	}

	struct sockaddr_in to = { 0 };
	to.sin_family = AF_INET;
	to.sin_port = htons(session.m_port);
	to.sin_addr = *(struct in_addr *) h->h_addr;

	const char *buffer = (const char *) builder.GetBuffer();
	int length = builder.GetSize();
	int result = sendto(s, buffer, length, 0, (struct sockaddr*) &to,
			sizeof(to));
	close(s);

	return result >= 0;
}

bool SnmpTrap::SendSpecialTrap(int specialTrapType) {
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
