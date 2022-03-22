/*
 * CHost.cc
 *
 *  Created on: 12.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CHost.h"

namespace SF {
namespace CORE {
namespace NET {

CHost::CHost() {
	// TODO Auto-generated constructor stub

}

CHost::CHost(const std::string& ip, int port) {
	this->setHost(ip, port);
}

CHost::~CHost() {
	// TODO Auto-generated destructor stub
}

void CHost::setHostIp(const std::string& ip) {
	this->m_host_ip = ip;
}

void CHost::setHostPort(int port) {
	this->m_host_port = port;
}

void CHost::setHost(const std::string& ip, int port) {
	this->setHostIp(ip);
	this->setHostPort(port);
}

const int CHost::getPort() const {
	return this->m_host_port;
}

const std::string CHost::getIp() const {
	return this->m_host_ip;
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */



