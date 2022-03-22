/*
 * CMonDeviceBlizoo.h
 *
 *  Created on: 18.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CMONDEVICEBLIZOO_H_
#define CMONDEVICEBLIZOO_H_

#include <softel/core/net/CSocketClient.h>

namespace SF {
namespace OSS {
namespace EXEC {

class CMonDeviceBlizoo {
public:
	CMonDeviceBlizoo(const std::string& host, int port);
	virtual ~CMonDeviceBlizoo();

	void connect();
	void disconnect();

	void parseResult();

	void dumpInfo();

private:

	std::string	m_host;
	int		m_port;

	SF::CORE::NET::CSocketClient	m_socket;

	int		m_period_connection;

	std::string	m_raw_response;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CMONDEVICEBLIZOO_H_ */
