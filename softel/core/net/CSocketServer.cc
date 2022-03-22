/*
 * CSocketServer.cc
 *
 *  Created on: 03.02.2013
 *      Author: STAN
 *
 *      www.softel.bg	All rights reserved
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSocketServer.h"
#include <softel/core/error/CErrorCommon.h>

using namespace SF::CORE::ERROR;

namespace SF {
namespace CORE {
namespace NET {

CSocketServer::CSocketServer() {
	// TODO Auto-generated constructor stub

}

CSocketServer::~CSocketServer() {
	// TODO Auto-generated destructor stub
}

/**
 * Set Server Socket Options - mainly SO REUSE ADDRESS
 *
 */
void CSocketServer::setSocketOptions() {

	int one = 1;

	if (setsockopt(this->getSocket(), SOL_SOCKET, SO_REUSEADDR, (char *) &one, sizeof(one)) < 0) {
		std::stringstream ss;
		ss << "Error:" << __FUNCTION__ << ":" << __LINE__;
		throw CErrorCommon(ss.str());
	}

}

/**
 * Initialize server socket and start listen
 */
void CSocketServer::initSocket() {

	struct sockaddr_in server_addr;

	initSocketDescriptor();
	this->setSocketOptions();

	// clear server socket
	bzero(&server_addr, sizeof(server_addr));

	// Set the address and port
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(this->getPort());

	if (bind(this->getSocket(), (struct sockaddr *) &server_addr, sizeof(server_addr))
			< 0) {
		std::stringstream ss;
		ss << "Error:" << __FUNCTION__ << ":" << __LINE__;
		throw CErrorCommon(ss.str());
	}

	if (listen(this->getSocket(), MAX_CONNECTIONS) < 0) {
		std::stringstream ss;
		ss << "Error:" << __FUNCTION__ << ":" << __LINE__;
		throw CErrorCommon(ss.str());
	}

}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

