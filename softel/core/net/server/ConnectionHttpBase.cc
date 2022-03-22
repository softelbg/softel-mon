/*
 * ConnectionHttpBase.cc
 *
 *  Created on: Dec 2, 2013
 *      Author: sgeorgiev
 */

#include <iostream>
#include "ConnectionHttpBase.h"
#include <vector>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>

#include "RequestHttpBase.h"
#include "ReplyHttpBase.h"

using namespace std;

namespace SF {
namespace CORE {
namespace NET {

ConnectionHttpBase::~ConnectionHttpBase() {
	// TODO Auto-generated destructor stub
}

ConnectionHttpBase::ConnectionHttpBase(boost::asio::io_service& io_service,
		boost::shared_ptr<RequestHandlerBase> handler) :
		ConnectionTcpBase(io_service) {

	// TODO: do not call base ctor due to duplicate request init...
	m_request_handler = handler;
	m_request.reset(new RequestHttpBase());
	m_reply.reset(new ReplyHttpBase());
}

void ConnectionHttpBase::start() {
	m_socket.async_read_some(boost::asio::buffer(m_buffer),
			m_strand.wrap(
					boost::bind(&ConnectionHttpBase::handleRead,
							static_pointer_cast<ConnectionHttpBase>(shared_from_this()),
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred)));
}

void ConnectionHttpBase::handleRead(const boost::system::error_code& e,
		std::size_t bytes_transferred) {

	if (!e) {
		boost::tribool result;
		boost::tie(result, boost::tuples::ignore) = m_request_parser.parse(
				m_request, m_buffer.data(), m_buffer.data() + bytes_transferred);

		m_request->postParse();

		// TODO: Pass the socket endpoint info to the request
		try {
			//m_request->setRemoteAddress(getSocket().remote_endpoint().address());
			boost::asio::ip::tcp::endpoint remote_ep = getSocket().remote_endpoint();
			boost::asio::ip::address remote_ad = remote_ep.address();
			std::string remoteIp = remote_ad.to_string();
			m_request->setRemoteIp(remoteIp);
			cout << __FILE__ << "::" << __FUNCTION__ << " remote address: " << remoteIp << endl;
		} catch (exception& e) {
			cout << __FILE__ << "::" << __FUNCTION__ << " EXCEPTION: " << e.what() << endl;
		}

		cout << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;
		cout << " res: " << result << " request: " << m_request->toString() << endl;

		if (result) {
			m_request_handler->handleRequest(m_request, m_reply);
			boost::asio::async_write(m_socket, m_reply->toBuffers(),
					m_strand.wrap(
							boost::bind(&ConnectionHttpBase::handleWrite,
									static_pointer_cast<ConnectionHttpBase>(shared_from_this()),
									boost::asio::placeholders::error)));
		} else if (!result) {
			m_reply = ReplyHttpBase::stockReply(ReplyHttpBase::bad_request);
			boost::asio::async_write(m_socket, m_reply->toBuffers(),
					m_strand.wrap(
							boost::bind(&ConnectionHttpBase::handleWrite,
									static_pointer_cast<ConnectionHttpBase>(shared_from_this()),
									boost::asio::placeholders::error)));
		} else {
			m_socket.async_read_some(boost::asio::buffer(m_buffer),
					m_strand.wrap(
							boost::bind(&ConnectionHttpBase::handleRead,
									static_pointer_cast<ConnectionHttpBase>(shared_from_this()),
									boost::asio::placeholders::error,
									boost::asio::placeholders::bytes_transferred)));
		}
	}

	// If an error occurs then no new asynchronous operations are started. This
	// means that all shared_ptr references to the ConnectionHttpBase object will
	// disappear and the object will be destroyed automatically after this
	// handler returns. The ConnectionHttpBase class's destructor closes the socket.
}

void ConnectionHttpBase::handleWrite(const boost::system::error_code& e) {
	if (!e) {
		// Initiate graceful ConnectionHttpBase closure.
		boost::system::error_code ignored_ec;
		m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
				ignored_ec);
	}

	// No new asynchronous operations are started. This means that all shared_ptr
	// references to the ConnectionHttpBase object will disappear and the object will be
	// destroyed automatically after this handler returns. The ConnectionHttpBase class's
	// destructor closes the socket.
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
