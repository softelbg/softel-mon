/*
 * ServerHttpStandard.cc
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#include "ServerHttpStandard.h"
#include "RequestHandlerHttp.h"

namespace SF {
namespace CORE {
namespace NET {

ServerHttpStandard::ServerHttpStandard(const std::string& address, const std::string& port,
	    const std::string& doc_root, std::size_t thread_pool_size)
	: ServerHttpBase(address, port, thread_pool_size),
	  m_doc_root(doc_root) {

	m_request_handler.reset(new RequestHandlerHttp(doc_root));
}

ServerHttpStandard::~ServerHttpStandard() {
	// TODO Auto-generated destructor stub
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
