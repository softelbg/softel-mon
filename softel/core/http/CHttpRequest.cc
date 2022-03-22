/*
 * CHttpRequest.cc
 *
 *  Created on: 20.05.2012
 *      Author: STAN
 *
 *		www.softel.bg
 *
 *		Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CHttpRequest.h"

#include <softel/core/net/CSocketClient.h>

using namespace std;
using namespace SF::CORE::NET;

namespace SF {
namespace CORE {
namespace HTTP {

CHttpRequest::CHttpRequest() {
	this->setPort(0);
}

CHttpRequest::CHttpRequest(const string& url) {
	this->setPort(0);
	setUrl(url);
}

CHttpRequest::CHttpRequest(const string& host, const int port, const string& url) {
	setHost(host);
	setPort(port);
	setUrl(url);
}

CHttpRequest::CHttpRequest(const CHost& host, const string& url) {
	setHost(host.getIp());
	setPort (host.getPort());
	setUrl(url);
}

CHttpRequest::~CHttpRequest() {
}

void CHttpRequest::setUrl(const string& url) {
	m_url = url;
}

CHttpResponse& CHttpRequest::getHttpResponse() {
	return m_httpResponse;
}

/**
 * Do HTTP GET request and set response.
 */
bool CHttpRequest::doGet() {

	bool result;

	this->setRequest("GET " + m_url);

	result = this->doRequest();

	m_httpResponse.setResponse(this->getResponse());

	if (result == false) {
		m_httpResponse.setError(this->getResponse());
	}

	return result;
}

bool CHttpRequest::doPost() {

	return true;
}

} /* namespace HTTP */
} /* namespace CORE */
} /* namespace SF */


