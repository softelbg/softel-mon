/*
 * CClientRequest.h
 *
 *	Client Request via Socket to a Host:Port
 *
 *  Created on: 06.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCLIENTREQUEST_H_
#define CCLIENTREQUEST_H_

#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <string>

#include "CHost.h"


namespace SF {
namespace CORE {
namespace NET {

class CClientRequest {

public:
	CClientRequest();
	CClientRequest(const std::string& host, int port);
	CClientRequest(const std::string& host, int port, const std::string& request);
	virtual ~CClientRequest();

	virtual void setHost(const std::string& host);
	virtual void setPort(int port);
	virtual void setRequest(const std::string& request);

	virtual std::string& getResponse();

	virtual bool doRequest();

protected:

	CHost	m_host;

	std::string m_request;
	std::string m_response;

};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

#endif /* CCLIENTREQUEST_H_ */
