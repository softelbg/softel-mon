/*
 * CHttpResponse.h
 *
 *	HTTP Response. Should parse the header and body.
 *
 *
 *  Created on: 20.05.2012
 *      Author: STAN
 *
 *		www.softel.bg
 *
 *		Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CHTTPRESPONSE_H_
#define CHTTPRESPONSE_H_

#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <string>

#include "CHttpHeader.h"

namespace SF {
namespace CORE {
namespace HTTP {

class CHttpResponse {

public:

	CHttpResponse();
	CHttpResponse(const std::string& response);

	void setResponse(const std::string& response);
	void setError(const std::string& error);

	CHttpHeader& getHeader();
	std::string& getBody();
	std::string& getError();

	bool hasErrors();


private:

	void parseResponse();

	std::string m_response;

	CHttpHeader m_header;

	std::string m_body;
	std::string m_error;

};

} /* namespace HTTP */
} /* namespace CORE */
} /* namespace SF */


#endif /* CHTTPRESPONSE_H_ */
