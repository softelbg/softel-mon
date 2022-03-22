/*
 * CSocket.h
 *
 *	Socket Representation class
 *
 *  Created on: 08.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg	All rights reserved
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSOCKET_H_
#define CSOCKET_H_

#include <strings.h>
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stddef.h>

#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <string>

#include <softel/core/CObject.h>

namespace SF {
namespace CORE {
namespace NET {

#define MAX_CONNECTIONS 64

#define DEFAULT_WAIT_TIMEOUT 10
#define DEFAULT_RESPONSE_TIMEOUT 30
#define DEFAULT_INTERPACKET_TIMEOUT 15

#define MAXDATASIZE 1048576
#define PACKET_SIZE 4096
#define TCP_SOCK_BUFFER 32760
#define RFC1323_ON 1
#define REUSEADDR_ON 1
#define END_OF_PACKET 0x3


class CSocket: public SF::CORE::CObject {

public:

	CSocket();
	CSocket(int sock_fd);
	virtual ~CSocket();

	virtual void initSocket();

	void receiveData(std::string& received);
	void sendData(const std::string& response);

	void closeSocket();

	void setSocket(int sock_fd);
	void setPort(int port);
	void setIp(const std::string& ip);
	void setWaitPeriod(int seconds, int useconds);
	int getSocket();
	int getPort();
	std::string getIp();

	std::string toString();

	void dumpInfo();

protected:

	virtual void setSocketOptions();

	void initSocketDescriptor();

private:

	int m_sock_fd;

	int m_port;
	std::string m_ip;

	int m_wait_period_seconds;
	int m_wait_period_microseconds;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

#endif /* CSOCKET_H_ */
