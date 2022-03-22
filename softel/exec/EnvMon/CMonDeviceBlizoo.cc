/*
 * CMonDeviceBlizoo.cc
 *
 *  Created on: 18.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CMonDeviceBlizoo.h"
#include <softel/core/error/CErrorCommon.h>
#include <softel/core/error/CErrorSql.h>

using namespace std;
using namespace SF::OSS::EXEC;
using namespace SF::CORE::ERROR;

namespace SF {
namespace OSS {
namespace EXEC {

CMonDeviceBlizoo::CMonDeviceBlizoo(const string& host, int port) {
	this->m_host = host;
	this->m_port = port;
}

CMonDeviceBlizoo::~CMonDeviceBlizoo() {

	this->m_socket.closeSocket();

}

void CMonDeviceBlizoo::connect() {

	this->m_socket.setIp(this->m_host);
	this->m_socket.setPort(this->m_port);

	this->m_socket.setWaitPeriod(1, 0);

	try {

		string response;

		this->m_socket.initSocket();

		for(int i = 0; i < 4; i++) {

			this->m_socket.receiveData(response);

			cout << "response[" << i << "] = [" << response << "]" << endl;

			this->m_raw_response += response;

		}


	} catch (CErrorCommon e) {
		cout << __FUNCTION__ << ": Error: " << e.getMessage() << endl;
	}

}

void CMonDeviceBlizoo::disconnect() {

	this->m_socket.closeSocket();

}

void CMonDeviceBlizoo::parseResult() {
}

void CMonDeviceBlizoo::dumpInfo() {
	cout << __FILE__ << ": raw result:" << endl << this->m_raw_response << endl;
}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */


