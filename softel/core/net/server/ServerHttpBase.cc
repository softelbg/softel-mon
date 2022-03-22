/*
 * ServerHttpBase.cc
 *
 *  Created on: Dec 2, 2013
 *      Author: sgeorgiev
 */

#include "ServerHttpBase.h"
#include "ConnectionHttpBase.h"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <iostream>

using namespace std;

namespace SF {
namespace CORE {
namespace NET {

ServerHttpBase::~ServerHttpBase() {
	// TODO Auto-generated destructor stub
}


ServerHttpBase::ServerHttpBase(const std::string& address, const std::string& port,
    std::size_t thread_pool_size)
  : ServerTcpBase(address, port, thread_pool_size) {

	m_request_handler.reset(new RequestHandlerBase());
}

void ServerHttpBase::startAccept() {

	cout << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << endl;

	m_new_connection.reset(new ConnectionHttpBase(m_io_service, m_request_handler));
	m_acceptor.async_accept(m_new_connection->getSocket(),
			boost::bind(&ServerHttpBase::handleAccept, this,
					boost::asio::placeholders::error));
}

void ServerHttpBase::handleAccept(const boost::system::error_code& e) {

	if (!e) {
		m_new_connection->start();
	}

	startAccept();
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
