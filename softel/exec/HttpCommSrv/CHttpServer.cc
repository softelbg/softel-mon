/*
 * CHttpServer.cc
 *
 *  Created on: 08.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include <string.h>
#include "CHttpServer.h"

#include <softel/core/net/CSocketServer.h>
#include <softel/core/net/CSocketClient.h>
#include <softel/core/error/CErrorCommon.h>
#include <softel/core/system/CSystemTools.h>

#include "CThrConnection.h"
#include "CThrChildWaiter.h"
#include "HttpSrvCommon.h"
#include "CHttpCmdSrvLog.h"

using namespace std;
using namespace SF::CORE::NET;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace OSS {
namespace EXEC {

CHttpServer::CHttpServer() {

}

CHttpServer::CHttpServer(int port) {

	setPort(port);
}

CHttpServer::~CHttpServer() {

}

void CHttpServer::setPort(int port) {
	m_serverPort = port;
}

/**
 * Start the Server
 */
void CHttpServer::startServer() {

	while (true) {

		try {
			startListen();
		} catch (CErrorCommon &error) {
			LOGHCS(LOG_LEVEL_ERROR, error.getMessage());
		}

		CSystemTools::doSleep(1);
	}

}

/**
 * Start loop for the server and listen for connections.
 */
void CHttpServer::startListen() {

	CSocketServer serverSocket;
	serverSocket.setPort(m_serverPort);
	serverSocket.initSocket();

	CSocket acceptedSocket;

	CThrChildWaiter thrWaiter;
	thrWaiter.start(THR_STACK_SIZE, true);

	while (true) {

		acceptConnection(serverSocket, acceptedSocket);

		// Start worker thread here...

		//CThrConnection* pThr = new CThrConnection(acceptedSocket);
		//pThr->start(THR_STACK_SIZE, true);
		CThrConnection fThr(acceptedSocket);
		fThr.startForked();

	}


}

/**
 * Accept new Connection. Make client connection socket.
 */
void CHttpServer::acceptConnection(CSocket& serverSocket, CSocket& acceptedSocket) {

	int client_sockfd;
	int accepted_port;
	string accepted_ip;
	struct sockaddr_in client_addr;
	unsigned ca_len;

	memset (&client_addr, '\0', sizeof (client_addr));
	ca_len = sizeof (client_addr);

	if ((client_sockfd = accept(serverSocket.getSocket(), (struct sockaddr *) &client_addr, &ca_len)) < 0) {
		serverSocket.closeSocket();
	 	stringstream ss;
		ss << "Error:" << __FUNCTION__ << ":" << __LINE__;	
		throw CErrorCommon(ss.str());
	}

	accepted_port = ntohs (client_addr.sin_port);
	accepted_ip = (char *) inet_ntoa (client_addr.sin_addr);

	acceptedSocket.setSocket(client_sockfd);
	acceptedSocket.setIp(accepted_ip);
	acceptedSocket.setPort(accepted_port);

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */


