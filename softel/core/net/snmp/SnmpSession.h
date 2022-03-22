/*
 * SnmpSession.h
 *
 *  Created on: 20.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef SNMPSESSION_H_
#define SNMPSESSION_H_

#include <softel/core/CObject.h>

namespace SF {
namespace CORE {
namespace NET {

#define SNMP_PATH_RECEIVER        "snmp.receiver."
#define SNMP_ENTERPRISE           "1.3.6.1.4.1.6876"
#define SNMP_TRAP_OID             SNMP_ENTERPRISE ".4.3"


static const int SNMP_COLDSTART_TRAP = 0;
static const int SNMP_TEST_TRAP = 1;
static const int SNMP_ALARM_TRAP = 2;
static const int SNMP_EVENT_TRAP = 3;

/*Trap types we currently support*/

static const int ASN_TRAP_GENERIC_CS_TYPE = 0;   // Generic Cold Start trap
static const int ASN_TRAP_GENERIC_ENT_TYPE = 6; // Generic Enterprise-specific trap
static const int ASN_TRAP_SPECIFIC_ALARM = 203; // Specific Alarm type
static const int ASN_TRAP_SPECIFIC_TEST = 202; // Specific Test type
static const int ASN_TRAP_SPECIFIC_EVENT = 204; // Specific Event type

/* SNMP DateAndTime length in octets */
static const int SNMP_DATETIME_LEN = 11;

static const int SNMP_BUFFER_SIZE = 0x2000; // 8k

static const int ASN_TYPE_INTEGER = 0x02;
static const int ASN_TYPE_STRING = 0x04;
static const int ASN_TYPE_OID = 0x06;
static const int ASN_TYPE_SEQUENCE = 0x30;
static const int ASN_TYPE_IP = 0x40;
static const int ASN_TYPE_TIME = 0x43;
static const int ASN_TYPE_TRAP = 0xA4;

static const int ASN_TRAP_PORT = 162;
static const int ASN_TRAP_SNMPV1 = 1;

/*
 *
 */
class SnmpSession: public SF::CORE::CObject {
public:
	SnmpSession();
	virtual ~SnmpSession();

	long m_version;        			// SNMP version. 1-> SNMPv1, 2-> SNMPv2

	std::string m_destination;   	// where to send the trap
	std::string m_community;      	// community on destination
	long m_port;           			// port number on destination

	std::string m_enterprise;
	std::string m_agent;
	long m_traptype;
	long m_timestamp;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* SNMPSESSION_H_ */
