/*
 * ServerHttpCmdBase.cc
 *
 *  Created on: Dec 6, 2013
 *      Author: sgeorgiev
 */

#include "ServerHttpCmdBase.h"
#include "RequestHandlerHttpCmdBase.h"

namespace SF {
namespace CORE {
namespace NET {

ServerHttpCmdBase::ServerHttpCmdBase(
			const std::string& address,
			const std::string& port,
			std::size_t thread_pool_size)
	: ServerHttpBase(address, port, thread_pool_size) {

	m_request_handler.reset(new RequestHandlerHttpCmdBase(true));
}

ServerHttpCmdBase::~ServerHttpCmdBase() {
	// TODO Auto-generated destructor stub
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
