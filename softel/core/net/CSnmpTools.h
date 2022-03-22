/*
 * CSnmpTools.h
 *
 *	Snmp tools
 *
 *  Created on: 11.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSnmpTools_H
#define CSnmpTools_H

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

namespace SF {
namespace CORE {
namespace NET {

class CSnmpTools {
public:

	CSnmpTools();
	CSnmpTools(const std::string& ip);

	~CSnmpTools();

	bool SnmpGetAsynch(const char *snmp, const char *community, char *snmpResult);


	bool SnmpGet(const std::string& snmp, char* snmpResult);
	bool SnmpGet(const std::string& snmp, const struct snmp_session* sess_handle,
			char* snmpResult);
	bool SnmpGet(const char *snmp, struct snmp_session* sess_handle,
			char* snmpResult);
	bool SnmpGet(const char *snmp, const char *community, char *snmpResult);

	bool SnmpGetMultiple(const std::vector<std::string>& snmpOid, const char *community,
			std::vector<std::string>* snmpResult);

	bool SnmpWalk(const char *snmp, const char *community,
			std::vector<std::string> *snmpOID, std::vector<std::string> *snmpResult);

	bool SnmpWalkBulk(const std::string& snmp, std::vector<std::string>* snmpOID,
			std::vector<std::string>* snmpResult);
	bool SnmpWalkBulk(const std::string& snmp, const std::string& community,
			std::vector<std::string>* snmpOID, std::vector<std::string>* snmpResult);
	bool SnmpWalkBulk(const char* snmp, netsnmp_session* sess_handle,
			std::vector<std::string>* snmpOID, std::vector<std::string>* snmpResult);

	bool InitSnmpSession(const char *community,
			struct snmp_session** sess_handle);
	bool InitSnmpSession(const char *community);
	bool InitSnmpSession(const std::string& community);
	bool CloseSnmpSession();
	bool CloseSnmpSession(struct snmp_session* sess_handle);

	void setIp(const std::string& ip);

private:

	std::string	m_ip;
	char	m_ip_c[32];

	struct snmp_session* m_sess_handle;

};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

#endif
