/*
 * Host.h
 *
 *  Created on: Nov 22, 2013
 *      Author: stan
 */

#ifndef HOST_H_
#define HOST_H_

#include <softel/core/CObject.h>
#include <string>
#include <boost/asio.hpp>

namespace SF {
namespace CORE {
namespace NET {

class Host: public SF::CORE::CObject {
public:
	Host(const std::string& hostName, const std::string& hostPort, boost::asio::io_service& iosvc);
	virtual ~Host();

	void resolve();

	std::string getHostName() const {
		return m_hostname;
	}

	void setHostName(std::string hostname) {
		m_hostname = hostname;
	}

	std::string getHostPort() const {
		return m_hostport;
	}

	void setHostPort(std::string hostport) {
		m_hostport = hostport;
	}

	boost::asio::ip::tcp::resolver::iterator& getEndpoint() {
		return this->m_endpoint_iterator;
	}

private:
	std::string m_hostname;
	std::string m_hostport;

	boost::asio::io_service& m_io_service;
	boost::asio::ip::tcp::resolver::iterator m_endpoint_iterator;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* HOST_H_ */
