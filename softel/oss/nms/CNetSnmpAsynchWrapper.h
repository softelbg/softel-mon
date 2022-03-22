/*
 * CNetSnmpAsynchWrapper.h
 *
 *	Net-Snmp Asynch Wrapper
 *
 *  Created on: 27.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CNETSNMPASYNCHWRAPPER_H_
#define CNETSNMPASYNCHWRAPPER_H_

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/types.h>
#include <net-snmp/library/large_fd_set.h>

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "BNetworkElementBase.h"
#include "CNetworkElementList.h"
#include "BSnmpOidListBase.h"

#include <softel/oss/cmts/CSnmpOid.h>
#include <softel/oss/cmts/CSnmpValue.h>
#include <softel/oss/cmts/CSnmpValueSet.h>
#include <softel/core/system/CSystemTools.h>

namespace SF {
namespace OSS {
namespace NMS {

class CNetSnmpAsynchWrapper {
public:
	CNetSnmpAsynchWrapper();
	virtual ~CNetSnmpAsynchWrapper();

	bool snmpMultipleGetAsynch(	std::vector<BNetworkElementBase*>& listNetElem,
								std::vector<SF::OSS::CMTS::CSnmpOid>& oidList);

	bool snmpMultipleGetAsynch(	CNetworkElementList& listNetElem,
								std::vector<SF::OSS::CMTS::CSnmpOid>& oidList,
								const int maxCmCount);

	void initialize(void);
	bool finalize(struct session* hs[], int size);

	static int processResult(int status, struct snmp_session *sp, struct snmp_pdu *pdu, char* snmpResult);
	static int asynchResponse(int operation, struct snmp_session *sp, int reqid, struct snmp_pdu *pdu, void *magic);
	static void decNumActiveHosts(int* pActiveHosts, pthread_mutex_t* mutex);

private:

	//void initialize();

	struct snmp_session* m_sess_handle;

	int  m_active_hosts;
	pthread_mutex_t m_mutex;

};

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CNETSNMPASYNCHWRAPPER_H_ */
