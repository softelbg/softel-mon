/*
 * CSnmpToolsAsynch.h
 *
 *	SNMP Asynch requests
 *
 *  Created on: 03.11.2011
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSnmpToolsAsynch_H
#define CSnmpToolsAsynch_H

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

#include "CSnmpOid.h"
#include "CCmtsToolsSql.h"
#include "CCmts.h"
#include "CCmtsCm.h"
#include "SnmpOids.h"
#include "CSnmpValue.h"

#include <softel/core/inc/ErrorMsg.h>
#include <softel/core/error/CErrorCommon.h>
#include <softel/db/CSqlDbFactory.h>
#include <softel/core/net/CIp.h>
#include <softel/db/CMySqlDB.h>


namespace SF {
namespace OSS {
namespace CMTS {

class CSnmpToolsAsynch {
public:

	CSnmpToolsAsynch();
	CSnmpToolsAsynch(const std::string& ip);

	~CSnmpToolsAsynch();


	bool snmpMultipleGetAsynch(	map<std::string, CCmtsCm*>& cmList,
								vector<CSnmpOid>& oidList);
	bool snmpMultipleGetAsynch(	vector<CCmtsCm*>& cmList,
								vector<CSnmpOid>& oidList,
								const int maxCmCount);
	bool snmpMultipleGetAsynch(	vector<CCmtsCm*>& cmList,
								vector<CSnmpOid>& oidList);

	bool saveSnmpValues(struct session* hs[], int size);

	void initialize(void);

	static int processResult(	int status, struct snmp_session *sp,
								struct snmp_pdu *pdu, char* snmpResult);

	static int asynchResponse(	int operation, struct snmp_session *sp, int reqid,
			    		struct snmp_pdu *pdu, void *magic);

	static void decNumActiveHosts(int* pActiveHosts, pthread_mutex_t* mutex);

	void setIp(const std::string& ip);


private:

	std::string	m_ip;

	std::string	m_rcommunity;

	struct snmp_session* m_sess_handle;

	int  m_active_hosts;
	pthread_mutex_t m_mutex;
};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif
