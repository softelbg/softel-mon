/*
 * CSocket.cc
 *
 *  Created on: 08.02.2012
 *      Author: sgeorgiev
 *
 *	www.softel.bg	All rights reserved
 *
 *	Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 *  $Id:  Exp $
 *
 */

#include "CSocket.h"

#include <softel/core/inc/ErrorMsg.h>
#include <softel/core/error/CErrorCommon.h>

#include <unistd.h>
#include <string.h>

using namespace SF::CORE::ERROR;
using namespace std;

namespace SF {
namespace CORE {
namespace NET {

CSocket::CSocket() {

	setWaitPeriod(0, 100000);

}

CSocket::CSocket(int sock_fd) :
	m_sock_fd(sock_fd) {

	setWaitPeriod(0, 100000);

}

CSocket::~CSocket() {

	closeSocket();

}

void CSocket::setSocket(int sock_fd) {
	m_sock_fd = sock_fd;
}

int CSocket::getPort() {
	return m_port;
}

int CSocket::getSocket() {
	return m_sock_fd;
}

void CSocket::setPort(int port) {
	m_port = port;
}

void CSocket::setIp(const std::string& ip) {
	m_ip = ip;
}

/**
 * Set socket options.
 */
void CSocket::setSocketOptions() {

}

void CSocket::setWaitPeriod(int seconds, int useconds) {
	this->m_wait_period_seconds = seconds;
	this->m_wait_period_microseconds = useconds;
}

void CSocket::initSocket() {

	initSocketDescriptor();

}

std::string CSocket::getIp() {
	return this->m_ip;
}

void CSocket::initSocketDescriptor() {

	this->m_sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (this->m_sock_fd < 0) {
		std::stringstream ss;
		ss << "Error:" << __FUNCTION__ << ":" << __LINE__;
		throw CErrorCommon(ss.str());
	}

}

void CSocket::receiveData(std::string& receivedString) {
	char packet[PACKET_SIZE];
	int nbytes = 0, packet_len = 0;
	fd_set fds;
	struct timeval tv;

	char buffer[MAXDATASIZE];

	//cout << __FILE__ <<  __FUNCTION__ << ":" << __LINE__ << endl;
	
	FD_ZERO(&fds);
	FD_SET(m_sock_fd, &fds);
	tv.tv_sec = DEFAULT_RESPONSE_TIMEOUT;
	tv.tv_usec = 0;

	if (select(m_sock_fd + 1, &fds, NULL, NULL, &tv) <= 0) {
		std::stringstream ss;
		ss << "Error:" << __FUNCTION__ << ":" << __LINE__;
		ss << ":client request timeout expired " << DEFAULT_RESPONSE_TIMEOUT
				<< " sec";
		throw CErrorCommon(ss.str());
	}

	/* wait to start receiving */
	nbytes = recv(m_sock_fd, buffer, PACKET_SIZE, 0);

	//cout << __FILE__ <<  __FUNCTION__ << ":" << __LINE__ << " nbytes: "<< nbytes << endl;

	if (!nbytes) {
		std::stringstream ss;
		ss << "Error:" << __FUNCTION__ << ":" << __LINE__;
		ss << ":connection failed";
		throw CErrorCommon(ss.str());
	}

	tv.tv_sec = this->m_wait_period_seconds;
	tv.tv_usec = this->m_wait_period_microseconds;

	/* first packet received - check for more */
	for (;;) {
		if (buffer[nbytes - 1] == END_OF_PACKET) {
			//cout << __FILE__ <<  __FUNCTION__ << ":" << __LINE__ << " END_OF_PACKET" << endl;
			break;
		}

		if (select(m_sock_fd + 1, &fds, NULL, NULL, &tv) <= 0) {
			//cout << __FILE__ <<  __FUNCTION__ << ":" << __LINE__ << " select" << endl;
			break;
		}

		if (!(packet_len = recv(m_sock_fd, packet, PACKET_SIZE, 0))) {
			//cout << __FILE__ <<  __FUNCTION__ << ":" << __LINE__ << " packet_len: " << packet_len << endl;
			//cout << "error: packet receive error";
			break;
		}

		if (nbytes > MAXDATASIZE - packet_len - 1) {
			std::stringstream ss;
			ss << "Error:" << __FUNCTION__ << ":" << __LINE__;
			ss << ":data overflow";
			throw CErrorCommon(ss.str());
		}

		packet[packet_len] = '\0';
		strncpy(&buffer[nbytes], packet, packet_len);
		nbytes += packet_len;
	}

	buffer[nbytes] = '\0';

	receivedString = buffer;

}

void CSocket::sendData(const std::string& response) {

	int lenSent = 0;

	lenSent = write(m_sock_fd, response.c_str(), response.size());

	cout << __FILE__ <<  __FUNCTION__ << ":" << __LINE__ << " response.size: " << response.size() << " lenSent: " << lenSent << endl;

	if (lenSent != response.size()) {
		std::stringstream ss;
		ss << "Error:" << __FUNCTION__ << ":" << __LINE__;
		throw CErrorCommon(ss.str());
	}

}

void CSocket::closeSocket() {
	close(m_sock_fd);
}

void CSocket::dumpInfo() {
	cout << toString() << endl;
}

std::string CSocket::toString() {
	stringstream ss;

	ss << "SockFd:" << m_sock_fd << " Port:" << m_port << " Ip:" << m_ip;

	return ss.str();
}


} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

