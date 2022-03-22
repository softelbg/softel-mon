/*
 * CClientRequest.cc
 *
 *  Created on: 06.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CClientRequest.h"

#include "CSocket.h"
#include "CSocketClient.h"
#include <softel/core/error/CErrorCommon.h>

using namespace SF::CORE::ERROR;

namespace SF {
namespace CORE {
namespace NET {

CClientRequest::CClientRequest() {
	this->setPort(0);
}

CClientRequest::CClientRequest(const std::string& host, int port) {
	this->setHost(host);
	this->setPort(port);
}

CClientRequest::CClientRequest(const std::string& host, int port,
		const std::string& request) {
	this->setHost(host);
	this->setPort(port);
	this->setRequest(request);
}

CClientRequest::~CClientRequest() {
	// TODO Auto-generated destructor stub
}

void CClientRequest::setHost(const std::string& host) {
	this->m_host.setHostIp(host);
}

void CClientRequest::setPort(int port) {
	this->m_host.setHostPort(port);
}

void CClientRequest::setRequest(const std::string& request) {
	this->m_request = request;
}

std::string& CClientRequest::getResponse() {
	return this->m_response;
}

bool CClientRequest::doRequest() {

	CSocketClient sockClient;

	sockClient.setPort(m_host.getPort());
	sockClient.setIp(m_host.getIp());

	sockClient.initSocket();

	try {

		sockClient.sendData(m_request);

		sockClient.receiveData(m_response);

	} catch (CErrorCommon &error) {

		m_response = std::string(error.getMessage());

		//cout << __FUNCTION__ << ":" << __LINE__ << " " << m_response << endl;

		return false;
	}

	return true;
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

