/*
 * CThrConnection.cc
 *
 *  Created on: 25.01.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */


#include "CThrConnection.h"
#include "HttpSrvCommon.h"
#include "CRequestParser.h"
#include <softel/core/error/CErrorCommon.h>
#include <softel/core/util/CTimeCounter.h>
#include <softel/core/system/CSystemTools.h>

using namespace std;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::NET;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace OSS {
namespace EXEC {

CThrConnection::CThrConnection() {

	setResponseHeader();

}

CThrConnection::CThrConnection(const CSocket& acceptedSocket) {

	m_socket = acceptedSocket;

	setResponseHeader();

}

CThrConnection::~CThrConnection() {

}

string& CThrConnection::getResponseHeader() {
	return m_request_response_header;
}

void CThrConnection::setResponseHeader() {
	m_request_response_header = "HTTP/1.1 200 OK\r\n";
}

/**
 * Start the connection as Forked Process instead of a thread.
 *
 */
void CThrConnection::startForked() {

	int pid = CSystemTools::doFork();

	if (pid != 0) {

		m_socket.closeSocket();

		return;
	} else {
		this->run();
		CSystemTools::doExit(0);
	}
}

int CThrConnection::run() {

	string response = "HTTP/1.1 200 OK\r\n\r\n";
	string receivedData;

	cout << "Thr:" << getThrId() << endl;

	m_socket.dumpInfo();

	CTimeCounter tcMain;

	response += tcMain.getCTimeNow() + "\n";

	// Receive request
	try {

		m_socket.receiveData(receivedData);

		CRequestParser parser(receivedData);

		parser.parseRequest();

		parser.dumpInfo();

		response += parser.getResponse();

	} catch (CErrorCommon &error) {

		response += string(error.getMessage());

		cout << __FUNCTION__ << ":" << __LINE__ << " " << error.getMessage() << endl;

	}

	// Send Response
	try {
		m_socket.sendData(response);
	} catch (CErrorCommon &error) {
		cout << __FUNCTION__ << ":" << __LINE__ << " " << error.getMessage() << endl;
	}

	m_socket.closeSocket();

	cout << "Thr:" << getThrId() << " Ended" << endl;

	return 0;
}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */
