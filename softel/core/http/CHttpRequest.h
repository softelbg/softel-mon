/*
 * CHttpRequest.h
 *
 *	HTTP Request. Do GET/POST HTTP Request and make proper HTTP Response.
 *
 *  Created on: 20.05.2012
 *      Author: STAN
 *
 *		www.softel.bg
 *
 *		Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CHTTPREQUEST_H_
#define CHTTPREQUEST_H_

#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <string>

#include "CHttpResponse.h"
#include "CHttpHeader.h"
#include <softel/core/net/CClientRequest.h>
#include <softel/core/net/CHost.h>

namespace SF {
namespace CORE {
namespace HTTP {

class CHttpRequest : public SF::CORE::NET::CClientRequest {

public:
	CHttpRequest();
	CHttpRequest(const std::string& url);
	CHttpRequest(const std::string& host, const int port, const std::string& url);
	CHttpRequest(const SF::CORE::NET::CHost& host, const std::string& url);
	virtual ~CHttpRequest();

	void setUrl(const std::string& url);

	CHttpResponse& getHttpResponse();

	bool doGet();
	bool doPost();

private:

	std::string 			m_url;
	CHttpResponse	m_httpResponse;

};

} /* namespace HTTP */
} /* namespace CORE */
} /* namespace SF */


#endif /* CHTTPREQUEST_H_ */
