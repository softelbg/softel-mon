/*
 * CHttpResponse.cc
 *
 *  Created on: 20.05.2012
 *      Author: STAN
 *
 *		www.softel.bg
 *
 *		Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CHttpResponse.h"

using namespace std;

namespace SF {
namespace CORE {
namespace HTTP {

CHttpResponse::CHttpResponse() {

}

CHttpResponse::CHttpResponse(const string& response) {
	setResponse(response);
}

/**
 * Set raw http response and parse it.
 */
void CHttpResponse::setResponse(const string& response) {

	m_response = response;

	parseResponse();

}

void CHttpResponse::setError(const string& error) {
	m_error = error;
}

CHttpHeader& CHttpResponse::getHeader() {
	return m_header;
}

string& CHttpResponse::getBody() {
	return m_body;
}

string& CHttpResponse::getError() {
	return m_error;
}

bool CHttpResponse::hasErrors() {
	return (m_error.size() > 0);
}

/**
 * Parse the raw http response.
 * Set proper http header, body
 */
void CHttpResponse::parseResponse() {

	string delim = "\r\n\r\n";
	size_t pos;

	pos = m_response.find(delim);

	if (pos != string::npos) {

		m_header.setHeader(m_response.substr(0, pos));

		m_body = m_response.substr(pos + delim.size());

	} else {

		m_body = m_response;

	}

}

} /* namespace HTTP */
} /* namespace CORE */
} /* namespace SF */

