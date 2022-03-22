/*
 * HttpSrvCommon.h
 *
 *  Created on: 10.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef HTTPSRVCOMMON_H_
#define HTTPSRVCOMMON_H_

namespace SF {
namespace OSS {
namespace EXEC {


//		Commands
#define CMD_PING			"ping"
#define CMD_RESET			"reset"
#define CMD_DHCP_LOG		"dhcplog"
#define CMD_SNMP_ALL		"snmpAll"
#define CMD_SNMP_IF_ALL		"snmpIfAll"
#define CMD_SNMP_CMTS_IF	"snmpCmtsIf"
#define CMD_SNMP_CM_SPEED	"snmpCmSpeed"
#define CMD_SNMP_CM_STATUS	"snmpCmStatus"
#define CMD_FORCE_PING		"forcePing"
#define CMD_SERVER_INFO		"serverInfo"


//		Params
#define PARAM_GET			"GET"
#define PARAM_POST			"POST"
#define PARAM_CMD			"cmd"
#define PARAM_IP			"ip"
#define PARAM_CM_IP			"cmIp"
#define PARAM_CMTS_IP		"cmtsIp"
#define PARAM_CMTS_ID		"cmtsId"
#define PARAM_MAC_ID		"macId"
#define PARAM_MAC			"mac"
#define PARAM_SOFIA			"sof"
#define PARAM_DHCP_SERVER	"dhcp_server"
#define PARAM_DHCP_PORT		"dhcp_server_port"


//		Thread Options
#define THR_STACK_SIZE 4194304


#define MSG_DHCPLOG_INAVLID_PARAMS	"No Dhcp Log available"


} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* HTTPSRVCOMMON_H_ */
