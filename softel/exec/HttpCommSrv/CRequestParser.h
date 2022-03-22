/*
 * CRequestParser.h
 *
 *  Created on: 09.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CREQUESTPARSER_H_
#define CREQUESTPARSER_H_


#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <string>

#include <softel/db/CSqlDbFactory.h>
#include <softel/oss/cmts/CCmtsCm.h>

namespace SF {
namespace OSS {
namespace EXEC {

class CRequestParser {

public:

	CRequestParser();
	CRequestParser(const std::string& request);
	~CRequestParser();


	void parseRequest();

	void parseHttpRequest(std::istringstream& iss);

	void tokenizeHttpReqParams(std::istringstream&);

	bool isHttp();
	bool isGet();
	bool isPost();

	void setResponse(const std::string&);
	void appendResponse(const std::string&);
	std::string& getResponse();

	void tokenizeString(const std::string&, const std::string&, std::vector<std::string>&); // TODO: Consider move to utility class

	void switchByCommand();

	void setParam(const std::string& key, const std::string& val);
	std::string getParam(const std::string& key);

	void dumpInfo();

private:

	void appendTimeStamp();
	void insertTimeStamp();

	std::string getParamMac();
	SF::OSS::CMTS::CCmtsCm* getCurrentCm();

	// Commands execute
	void doCmdPing();
	void doCmdForcePing();
	void doCmdDhcpLog();
	void doCmdReset();
	void doCmdSnmpAll();
	void doCmdSnmpIfAll();
	void doCmdSnmpCmtsIf();
	void doCmdSnmpCmSpeed();
	void doCmdSnmpCmStatus();
	void doCmdServerInfo();

	void initDbFactory();

	std::string m_request;
	std::string m_request_cmd; // main req command for ex. GET, POST...

	std::string m_request_response;

	bool m_is_http;
	bool m_is_get;
	bool m_is_post;


	std::map<std::string, std::string> m_map_params;

	SF::DB::CSqlDbFactory* 	m_dbFactory;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CREQUESTPARSER_H_ */
