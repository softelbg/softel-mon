/*
 * CCpe.cc
 *
 *  Created on: 23.04.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCpe.h"

using namespace std;

namespace SF {
namespace OSS {
namespace CMTS {

CCpe::CCpe() {

}

CCpe::CCpe(const string& ip, const string& mac) {

	setIp(ip);

	setMac(mac);

}

CCpe::~CCpe() {

}

void CCpe::setIp(const string& ip) {
	m_ip = ip;
}

void CCpe::setMac(const string& mac) {
	m_mac = mac;
}

string& CCpe::getIp() {
	return m_ip;
}

string& CCpe::getMac() {
	return m_mac;
}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */


