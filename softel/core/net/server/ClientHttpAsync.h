/*
 * ClientHttpAsync.h
 *
 *  Created on: Nov 22, 2013
 *      Author: stan
 */

#ifndef CLIENTHTTPASYNC_H_
#define CLIENTHTTPASYNC_H_

#include "ClientHttpBase.h"

namespace SF {
namespace CORE {
namespace NET {

class ClientHttpAsync: public SF::CORE::NET::ClientHttpBase {
public:
	ClientHttpAsync(const std::string& hostName, const std::string& hostPort,
			const std::string& urlpath, boost::asio::io_service& iosvc);
	virtual ~ClientHttpAsync();
};


} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* CLIENTHTTPASYNC_H_ */
