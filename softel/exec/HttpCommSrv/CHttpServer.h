/*
 * CHttpServer.h
 *
 *  Created on: 08.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CHTTPSERVER_H_
#define CHTTPSERVER_H_


#include <sstream>

#include <softel/core/net/CSocket.h>

namespace SF {
namespace OSS {
namespace EXEC {

class CHttpServer
{

public:

	CHttpServer();
	CHttpServer(int);
	~CHttpServer();

	void startServer();
	void startListen();

	void setPort(int port);

private:

	void acceptConnection(	SF::CORE::NET::CSocket& serverSocket,
							SF::CORE::NET::CSocket& acceptedSocket);

	int		m_serverPort;
	SF::CORE::NET::CSocket m_serverSocket;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CHTTPSERVER_H_ */
