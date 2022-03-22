/*
 * CSocketClient.cc
 *
 *  Created on: 03.02.2013
 *      Author: STAN
 *
 *      www.softel.bg	All rights reserved
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSocketClient.h"

#include <softel/core/error/CErrorCommon.h>

using namespace std;
using namespace SF::CORE::ERROR;

namespace SF {
namespace CORE {
namespace NET {

CSocketClient::CSocketClient() {
	// TODO Auto-generated constructor stub

}

CSocketClient::~CSocketClient() {
	// TODO Auto-generated destructor stub
}

/**
 * Set Client Socket Options - empty for now...
 *
 */
void CSocketClient::setSocketOptions() {


}

/**
 * Init Client Socket and connect
 */
void CSocketClient::initSocket() {

	struct hostent *h;

	struct sockaddr_in server_addr;

	initSocketDescriptor();
	this->setSocketOptions();

	// clear server socket
	bzero(&server_addr, sizeof(server_addr));

	if ((h = gethostbyname(this->getIp().c_str())) == NULL) {
		std::stringstream ss;
		ss << "Error:" << __FUNCTION__ << ":" << __LINE__;
		throw CErrorCommon(ss.str());
	}

	// Set the address and port
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr=*((struct in_addr*)h->h_addr);
	server_addr.sin_port = htons(this->getPort());

	if (connect(this->getSocket(), (struct sockaddr*) &server_addr, sizeof(struct sockaddr))
			== -1) {
		std::stringstream ss;
		ss << "Error:" << __FUNCTION__ << ":" << __LINE__;
		throw CErrorCommon(ss.str());
	}

}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
