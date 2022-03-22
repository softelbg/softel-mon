/*
 * ServerHttpCmdBase.h
 *
 *  Created on: Dec 6, 2013
 *      Author: sgeorgiev
 */

#ifndef SERVERHTTPCMDBASE_H_
#define SERVERHTTPCMDBASE_H_

#include "ServerHttpBase.h"

namespace SF {
namespace CORE {
namespace NET {

class ServerHttpCmdBase: public SF::CORE::NET::ServerHttpBase {
public:
	explicit ServerHttpCmdBase(	const std::string& address,
								const std::string& port,
								std::size_t thread_pool_size);
	virtual ~ServerHttpCmdBase();
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* SERVERHTTPCMDBASE_H_ */
