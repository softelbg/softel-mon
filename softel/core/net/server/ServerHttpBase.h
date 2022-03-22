/*
 * ServerHttpBase.h
 *
 *  Created on: Dec 2, 2013
 *      Author: sgeorgiev
 */

#ifndef SERVERHTTPBASE_H_
#define SERVERHTTPBASE_H_

#include "ServerTcpBase.h"
#include "RequestHandlerBase.h"
#include <boost/asio.hpp>
#include <string>


namespace SF {
namespace CORE {
namespace NET {

class ServerHttpBase : public ServerTcpBase {
public:
	explicit ServerHttpBase(const std::string& address, const std::string& port,
				std::size_t thread_pool_size);
	virtual ~ServerHttpBase();

protected:
	virtual void startAccept();
	virtual void handleAccept(const boost::system::error_code& e);

	// The handler for all incoming requests.
	boost::shared_ptr<RequestHandlerBase> m_request_handler;
	//request_handler m_request_handler;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* SERVERHTTPBASE_H_ */
