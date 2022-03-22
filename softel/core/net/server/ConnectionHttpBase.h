/*
 * ConnectionHttpBase.h
 *
 *  Created on: Dec 2, 2013
 *      Author: sgeorgiev
 */

#ifndef CONNECTIONHTTPBASE_H_
#define CONNECTIONHTTPBASE_H_

#include "ConnectionTcpBase.h"
#include <boost/shared_ptr.hpp>

#include "ReplyHttpBase.h"
#include "RequestHttpBase.h"
#include "HttpRequestParser.h"

namespace SF {
namespace CORE {
namespace NET {

class ConnectionHttpBase: 	public ConnectionTcpBase {
public:
	virtual ~ConnectionHttpBase();

	explicit ConnectionHttpBase(boost::asio::io_service& io_service,
			boost::shared_ptr<RequestHandlerBase> handler);

	void start();

protected:
	void handleRead(const boost::system::error_code& e,
			std::size_t bytes_transferred);

	void handleWrite(const boost::system::error_code& e);

	/// The parser for the incoming request.
	HttpRequestParser m_request_parser;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* CONNECTIONHTTPBASE_H_ */
