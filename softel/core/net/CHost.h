/*
 * CHost.h
 *
 *  Created on: 12.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CHOST_H_
#define CHOST_H_

#include <string>

namespace SF {
namespace CORE {
namespace NET {

class CHost {
public:
	CHost();
	CHost(const std::string& ip, int port);
	virtual ~CHost();

	void setHostIp(const std::string& ip);
	void setHostPort(int port);
	void setHost(const std::string& ip, int port);

	const int getPort() const;
	const std::string getIp() const;

private:
	std::string	m_host_ip;
	int		m_host_port;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

#endif /* CHOST_H_ */
