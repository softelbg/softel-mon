/*
 * CThrConnection.h
 *
 *  Created on: 25.01.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTHRCONNECTION_H_
#define CTHRCONNECTION_H_

#include <stdio.h>

#include <iostream>
#include <map>
#include <vector>
#include <string>


#include <softel/core/util/CThreads.h>

#include <softel/core/net/CSocket.h>

namespace SF {
namespace OSS {
namespace EXEC {


class CThrConnection : public SF::CORE::UTIL::CThreads
{

public:
	CThrConnection();
	CThrConnection(const SF::CORE::NET::CSocket& acceptedSocket);

	~CThrConnection();

	std::string& getResponseHeader();
	void setResponseHeader();

	void startForked();

protected:

	int run();

private:

	SF::CORE::NET::CSocket m_socket;

	std::string m_request_response_header;
	std::string m_request_response;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif
