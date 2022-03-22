/*
 * CRequestParser.cc
 *
 *  Created on: 09.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CRequestParser.h"
#include "CServerInfo.h"
#include "HttpSrvCommon.h"

#include <softel/core/net/CIpTools.h>
#include <softel/db/CSqlDbFactory.h>
#include <softel/oss/cmts/SqlTableNames.h>
#include <softel/oss/cmts/CCmtsCm.h>
#include <softel/oss/cmts/CCmtsIf.h>
#include <softel/core/net/CSocket.h>
#include <softel/core/util/CStringUtil.h>
#include <softel/core/http/CHttpRequest.h>
#include <softel/core/net/CPingCmtsCm.h>
#include <softel/oss/cmts/CMySqlConn.h>
#include <softel/oss/snmp/CSnmpCmIp.h>
#include <softel/core/error/CErrorCommon.h>
#include <softel/oss/cmts/CCmtsFileIni.h>
#include <softel/core/system/CSystemTools.h>

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::NET;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::SYSTEM;
using namespace SF::CORE::HTTP;
using namespace SF::OSS::CMTS;
using namespace SF::OSS::SNMP;

namespace SF {
namespace OSS {
namespace EXEC {

CRequestParser::CRequestParser() :
		m_is_http(false), m_is_get(false), m_is_post(false), m_dbFactory(NULL) {

}

CRequestParser::CRequestParser(const string& request) :
		m_is_http(false), m_is_get(false), m_is_post(false), m_dbFactory(NULL) {

	m_request = request;

}

CRequestParser::~CRequestParser() {

	delete m_dbFactory;

}

void CRequestParser::parseRequest() {

	istringstream iss(m_request);

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	string sub;

	iss >> sub;

	m_request_cmd = sub;

	// Check for GET request
	if (sub.compare(PARAM_GET) == 0) {
		m_is_http = true;
		m_is_get = true;

		parseHttpRequest(iss);

	} else if (sub.compare(PARAM_POST) == 0) {
		m_is_http = true;
		m_is_post = true;

		parseHttpRequest(iss);

	} else {
		// Not HTTP request, some other type...
		m_is_http = false;
	}

}

void CRequestParser::parseHttpRequest(istringstream& iss) {

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	if (isGet()) {
		tokenizeHttpReqParams(iss);
		switchByCommand();
	}

	this->insertTimeStamp();

}

/**
 * If there is a param cmd, make some action according to the command.
 *	TODO: Consider make a proper command handling class
 */
void CRequestParser::switchByCommand() {

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	string paramCmd = getParam(PARAM_CMD);

	// CMD Not Found
	if (paramCmd.size() == 0) {
		setResponse("No Command...");
		return;
	}

	try {

	if (paramCmd.compare(CMD_PING) == 0) {

		doCmdPing();

	} else if (paramCmd.compare(CMD_RESET) == 0) {

		doCmdReset();

	} else if (paramCmd.compare(CMD_SNMP_ALL) == 0) {

		doCmdSnmpAll();

	} else if (paramCmd.compare(CMD_SNMP_IF_ALL) == 0) {

		doCmdSnmpIfAll();

	} else if (paramCmd.compare(CMD_DHCP_LOG) == 0) {

		doCmdDhcpLog();

	} else if (paramCmd.compare(CMD_SNMP_CM_SPEED) == 0) {

		doCmdSnmpCmSpeed();

	} else if (paramCmd.compare(CMD_SNMP_CM_STATUS) == 0) {

		doCmdSnmpCmStatus();

	} else if (paramCmd.compare(CMD_FORCE_PING) == 0) {

		doCmdForcePing();

	} else if (paramCmd.compare(CMD_SERVER_INFO) == 0) {

		doCmdServerInfo();

	} else if (paramCmd.compare(CMD_SNMP_CMTS_IF) == 0) {

		doCmdSnmpCmtsIf();

	} else {
		setResponse("Unknown Command...");
	}

	} catch (CErrorCommon& e) {
		cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__;
		cout << " Error: " << e.getMessage() << endl;

		setResponse("Error: " + e.getMessage());
	}
}

/**
 * Ping CM
 */
void CRequestParser::doCmdPing() {

	string paramIp = getParam(PARAM_IP);
	string pingResult;

	// IP Not Found
	if (paramIp.size() == 0) {
		setResponse("No IP....");
		return;
	}

	CPingCmtsCm p(paramIp);
	p.run();

	setResponse(p.getFinalResult());

}

/**
 * Reset CM
 */
void CRequestParser::doCmdReset() {

	CIpTools ipTools;
	string paramIp = getParam(PARAM_IP);

	// IP Not Found
	if (paramIp.size() == 0) {
		setResponse("No IP....");
		return;
	}

	//string response = string("Reset ") + paramIp + string(" status ");
	//response += ipTools.snmpResetHost(paramIp);

	string response;

	if (ipTools.snmpResetHost(paramIp)) {
		response =
				"Модемът е рестартиран. Моля изчакайте връзката да се възстанови.";
	} else {
		response = "Модемът не е рестартиран. Моля опитайте ръчно.";
	}

	setResponse(response);

}

/**
 * Do DHCP Log Command.
 * Receive MAC, DhcpLog ip/port.
 * TODO: Hardcoded here the request pattern, consider move to .ini
 *
 */
void CRequestParser::doCmdDhcpLog() {

	string response;
	string request;

	string paramMac = this->getParamMac();
	string paramDhcpIp = getParam(PARAM_DHCP_SERVER);
	string paramDhcpPort = getParam(PARAM_DHCP_PORT);

	// Make param validation
	if (paramMac.size() == 0 || paramDhcpIp.size() == 0
			|| paramDhcpPort.size() == 0) {
		setResponse(MSG_DHCPLOG_INAVLID_PARAMS);
		return;
	}

	request = "?match=\"" + paramMac + "\"\r\n\r\n";

	int dhcpPort = atoi(paramDhcpPort.c_str());
	
	CHttpRequest httpRequest(paramDhcpIp, dhcpPort, request);

	httpRequest.doGet();

	response = httpRequest.getHttpResponse().getBody();
	response += httpRequest.getHttpResponse().getError();

	setResponse(response);
}

/**
 * Do Snmp All command. Make custom snmps for CM from CMTS/CM.
 */
void CRequestParser::doCmdSnmpAll() {

	string result;

	CCmtsCm* pCurrentCm = this->getCurrentCm();

	if (pCurrentCm) {

		pCurrentCm->setCmValidIp();

		pCurrentCm->dumpCmInfo();

		pCurrentCm->doSnmpGetAll(result);

		delete pCurrentCm;

	}

	setResponse(result);
}

/**
 * Do Snmp All command. Make custom snmps for IF from CMTS.
 */
void CRequestParser::doCmdSnmpIfAll() {

	string result;

	initDbFactory();

	string cmtsId = getParam(PARAM_CMTS_ID);
	string ifId = getParam(KEY_SNMP_OID_IF_ID);

	CCmtsIf currentIf(cmtsId);

	currentIf.setParam(KEY_SNMP_OID_IF_ID, ifId);

	currentIf.setAccountDb(m_dbFactory);

	currentIf.initFromDb();

	currentIf.dumpInfo();

	currentIf.doSnmpGetAll(result);

	setResponse(result);
}

/**
 * Do Snmp CMTS IF custom param...
 */
void CRequestParser::doCmdSnmpCmtsIf() {

	string result;

	initDbFactory();

	string cmtsId = getParam(PARAM_CMTS_ID);
	string ifId = getParam(KEY_SNMP_OID_IF_ID);
	string paramName = getParam("param");
	string oidId = getParam("oidId");

	CCmtsIf currentIf(cmtsId);

	currentIf.setParam(KEY_SNMP_OID_IF_ID, ifId);

	currentIf.setAccountDb(m_dbFactory);

	currentIf.initFromDb();

	currentIf.dumpInfo();

	currentIf.doSnmpGetCustomOid(oidId, result);

	setResponse(result);
}

/**
 * Do Snmp Get CM Speed command.
 */
void CRequestParser::doCmdSnmpCmSpeed() {

	string result;

	CCmtsCm* pCurrentCm = this->getCurrentCm();

	if (pCurrentCm) {
		pCurrentCm->doSnmpGetCmSpeed(result);
		delete pCurrentCm;
	}

	//setResponse(result);
	appendResponse(result);
}

/**
 * Do Snmp Get CM Status command.
 */
void CRequestParser::doCmdSnmpCmStatus() {

	string result;

	CCmtsCm* pCurrentCm = this->getCurrentCm();

	if (pCurrentCm) {
		pCurrentCm->doSnmpGetCmStatus(result);
		delete pCurrentCm;
	}

	setResponse(result);

}

/**
 * Append a Time Stamp to the Response.
 *
 */
void CRequestParser::appendTimeStamp() {
	this->appendResponse(CSystemTools::getCTimeNow());
}

/**
 * Insert a Time Stamp in the beginning of the response.
 *
 */
void CRequestParser::insertTimeStamp() {

	string timeStamp = CSystemTools::getCTimeNow() + "\n";

	this->m_request_response = timeStamp + this->m_request_response;
}

/**
 * Get correct MAC from param list.
 */
string CRequestParser::getParamMac() {

	string paramMac = getParam(PARAM_MAC);

	CStringUtil::replace(paramMac, "%3A", ":");

	return paramMac;
}

/**
 * Initialize and return new CM with current settings.
 */
CCmtsCm* CRequestParser::getCurrentCm() {

	initDbFactory();

	string cmtsId = getParam(PARAM_CMTS_ID);
	string macId = getParam(PARAM_MAC_ID);
	string cmIp = getParam(PARAM_CM_IP);
	string cmtsIp = getParam(PARAM_CMTS_IP);

	string paramMac = getParamMac();

	CCmtsCm* pCurrentCm = new CCmtsCm(macId, atoi(cmtsId.c_str()));

	pCurrentCm->setAccountDb(m_dbFactory);
	pCurrentCm->setParam(PARAM_MAC, paramMac);

	pCurrentCm->initFromDb();

	return pCurrentCm;
}

/**
 * Do a Forced Ping if the ip of a Cmts CM is not known.
 * Use CPE MAC to indentify the latest cmts registred IP.
 *
 */
void CRequestParser::doCmdForcePing() {

	CSnmpCmIp snmpCm;

	string cmtsId = getParam(KEY_COL_CMTS_ID);
	string paramMac = getParamMac();

	snmpCm.setParam(KEY_COL_CMTS_ID, cmtsId);
	snmpCm.setParam(KEY_SNMP_OID_CM_MAC_HEX, paramMac);

	snmpCm.init();

	this->setParam(PARAM_IP, snmpCm.getParam(KEY_COL_CM_IP));
	this->doCmdPing();
}

/**
 * Do some Server Info for maintenance purposes.
 * Should return used/free disk space, RAM, CPU and so on...
 * Using mostly shell command to retreive needed server info...
 */
void CRequestParser::doCmdServerInfo() {

	CServerInfo srvInfo;

	this->setResponse(srvInfo.getResult());

}

/**
 * Read DB properties from ini file and init Db Factory.
 */
void CRequestParser::initDbFactory() {

	if (m_dbFactory == NULL) {
		m_dbFactory = new CSqlDbFactory(CCmtsFileIni::getInstance()->get("MySQL", "Host"),
										CCmtsFileIni::getInstance()->get("MySQL", "User"),
										CCmtsFileIni::getInstance()->get("MySQL", "Pass"),
										CCmtsFileIni::getInstance()->get("MySQL", "Database")
										);
	}

}

void CRequestParser::appendResponse(const string& response) {
	m_request_response += response;
}
void CRequestParser::setResponse(const string& response) {
	m_request_response = response;
}
string& CRequestParser::getResponse() {
	return m_request_response;
}

bool CRequestParser::isHttp() {
	return m_is_http;
}

bool CRequestParser::isGet() {
	return m_is_get;
}
bool CRequestParser::isPost() {
	return m_is_post;
}

void CRequestParser::tokenizeHttpReqParams(istringstream& iss) {

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	vector<string> tokenParams;
	vector<string> tokenTool;

	unsigned int i;

	tokenTool.clear();
	tokenParams.clear();
	m_map_params.clear();

	tokenizeString(iss.str(), "?", tokenTool);

	if (tokenTool.size() >= 2) {
		tokenizeString(tokenTool[1], "&", tokenParams);
	}

	for (i = 0; i < tokenParams.size(); i++) {
		vector<string> param;

		tokenizeString(tokenParams[i], "=", param);

		if (param.size() == 2) {
			istringstream iss(param[1]);
			string val;
			iss >> val;

			//m_map_params[param[0]] = val;
			setParam(param[0], val);
		}
	}

}

/**
 * Tokenize string and fill vector with result tokens.
 * TODO: Move it to utility class...
 */
void CRequestParser::tokenizeString(const string& tokenizeMe,
		const string& delim, vector<string>& tokens) {

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	tokens.clear();
	size_t pos = 0;
	size_t posPrev = 0;

	while (true) {

		posPrev = pos;

		pos = tokenizeMe.find_first_of(delim, posPrev);

		string subToken = tokenizeMe.substr(posPrev, pos - posPrev);

		tokens.push_back(subToken);

		if (pos != string::npos) {

			pos++;

		} else {
			break;
		}

	}

}

void CRequestParser::setParam(const string& key, const string& val) {
	m_map_params[key] = val;
}

string CRequestParser::getParam(const string& key) {

	map<string, string>::iterator it;
	string val = "";

	it = m_map_params.find(key);

	if (it != m_map_params.end()) {
		val = it->second;
	}

	return val;
}

void CRequestParser::dumpInfo() {

	map<string, string>::iterator it;

	int i;

	cout << endl << __FILE__ << "  ----------  " << endl;
	//cout << "REQ:" << m_request << endl;
	//cout << "REQCMD:" << m_request_cmd << endl;
	cout << "REQRESPONSE:" << m_request_response << endl;

	cout << "Param Count:" << m_map_params.size() << endl;
	for (i = 0, it = m_map_params.begin(); it != m_map_params.end();
			it++, i++) {
		cout << "param[" << i << "] " << it->first << " = " << it->second
				<< endl;
	}

	cout << "  ----------  " << endl;
}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

