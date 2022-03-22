/*
 * ConnectionTcpBase.h
 *
 *  Created on: Dec 2, 2013
 *      Author: sgeorgiev
 */

#ifndef CONNECTIONTCPBASE_H_
#define CONNECTIONTCPBASE_H_

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "RequestBase.h"
#include "ReplyBase.h"
#include "RequestHandlerBase.h"

namespace SF {
namespace CORE {
namespace NET {

class ConnectionTcpBase: public boost::enable_shared_from_this<ConnectionTcpBase>,
		private boost::noncopyable {
public:
	// Construct a connection with the given io_service.
		explicit ConnectionTcpBase(boost::asio::io_service& io_service);
	virtual ~ConnectionTcpBase();

	// Get the socket associated with the connection.
	boost::asio::ip::tcp::socket& getSocket();

	// Start the first asynchronous operation for the connection.
	virtual void start();

protected:
	// Handle completion of a read operation.
	virtual void handleRead(const boost::system::error_code& e,
			std::size_t bytes_transferred);
	// Handle completion of a write operation.
	virtual void handleWrite(const boost::system::error_code& e);

	// Strand to ensure the connection's handlers are not called concurrently.
	boost::asio::io_service::strand m_strand;

	// Socket for the connection.
	boost::asio::ip::tcp::socket m_socket;

	// Buffer for incoming data.
	boost::array<char, 8192> m_buffer;

	// The handler used to process the incoming request.
	boost::shared_ptr<RequestHandlerBase> m_request_handler;

	// The incoming request.
	boost::shared_ptr<RequestBase> m_request;
	// The reply to be sent back to the client.
	boost::shared_ptr<ReplyBase> m_reply;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* CONNECTIONTCPBASE_H_ */
