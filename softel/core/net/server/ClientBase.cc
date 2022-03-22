/*
 * ClientBase.cc
 *
 *  Created on: Nov 22, 2013
 *      Author: stan
 */

#include "ClientBase.h"

using namespace SF::CORE;

namespace SF {
namespace CORE {
namespace NET {

ClientBase::ClientBase(const std::string& hostName, const std::string& hostPort, boost::asio::io_service& iosvc) :
	m_io_service(iosvc),
	m_socket(iosvc),
	m_host(hostName, hostPort, m_io_service) {

}

ClientBase::~ClientBase() {
	// TODO Auto-generated destructor stub
}

void ClientBase::connect() {
	boost::asio::connect(m_socket, m_host.getEndpoint());
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
