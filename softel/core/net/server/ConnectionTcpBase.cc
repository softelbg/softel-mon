/*
 * ConnectionTcpBase.cc
 *
 *  Created on: Dec 2, 2013
 *      Author: sgeorgiev
 */

#include <iostream>
#include "ConnectionTcpBase.h"
#include <vector>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace SF {
namespace CORE {
namespace NET {

ConnectionTcpBase::~ConnectionTcpBase() {
	// TODO Auto-generated destructor stub
}

ConnectionTcpBase::ConnectionTcpBase(boost::asio::io_service& io_service) :
		m_strand(io_service), m_socket(io_service) {

	m_request_handler.reset(new RequestHandlerBase());
	m_request.reset(new RequestBase());
	//m_reply.reset(new ReplytBase());
}

boost::asio::ip::tcp::socket& ConnectionTcpBase::getSocket() {
	return m_socket;
}

void ConnectionTcpBase::start() {
	m_socket.async_read_some(boost::asio::buffer(m_buffer),
			m_strand.wrap(
					boost::bind(&ConnectionTcpBase::handleRead,
							shared_from_this(),
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred)));
}

void ConnectionTcpBase::handleRead(const boost::system::error_code& e,
		std::size_t bytes_transferred) {
	if (!e) {

		const string stringReadResult(m_buffer.data(), bytes_transferred);

		cout << "Read: " << stringReadResult << endl;

		boost::asio::async_write(m_socket, boost::asio::buffer(m_buffer.data(), bytes_transferred),
				m_strand.wrap(boost::bind(&ConnectionTcpBase::handleWrite,
								shared_from_this(),
								boost::asio::placeholders::error)));
	}

	// If an error occurs then no new asynchronous operations are started. This
	// means that all shared_ptr references to the ConnectionTcpBase object will
	// disappear and the object will be destroyed automatically after this
	// handler returns. The ConnectionTcpBase class's destructor closes the socket.
}

void ConnectionTcpBase::handleWrite(const boost::system::error_code& e) {
	if (!e) {
		// Initiate graceful ConnectionTcpBase closure.
		boost::system::error_code ignored_ec;
		m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
				ignored_ec);
	}

	// No new asynchronous operations are started. This means that all shared_ptr
	// references to the ConnectionTcpBase object will disappear and the object will be
	// destroyed automatically after this handler returns. The ConnectionTcpBase class's
	// destructor closes the socket.
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
