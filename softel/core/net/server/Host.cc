/*
 * Host.cc
 *
 *  Created on: Nov 22, 2013
 *      Author: stan
 */

#include "Host.h"

namespace SF {
namespace CORE {
namespace NET {

Host::Host(const std::string& hostName, const std::string& hostPort, boost::asio::io_service& iosvc) :
	m_io_service(iosvc) {

	setHostName(hostName);
	setHostPort(hostPort);

	resolve();
}

Host::~Host() {
	// TODO Auto-generated destructor stub
}

void Host::resolve() {

	boost::asio::ip::tcp::resolver resolver(m_io_service);
	boost::asio::ip::tcp::resolver::query query(m_hostname, m_hostport);
	m_endpoint_iterator = resolver.resolve(query);

}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
