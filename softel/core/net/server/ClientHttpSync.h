/*
 * ClientHttpSync.h
 *
 *	Http Sync clinet based on Boost asio sync http client example
 *
 *  Created on: Nov 22, 2013
 *      Author: stan
 */

#ifndef CLIENTHTTPSYNC_H_
#define CLIENTHTTPSYNC_H_

#include "ClientHttpBase.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace SF {
namespace CORE {
namespace NET {

class ClientHttpSync: public SF::CORE::NET::ClientHttpBase {
public:
	ClientHttpSync(const std::string& hostName, const std::string& hostPort,
			const std::string& urlpath, boost::asio::io_service& iosvc);
	virtual ~ClientHttpSync();

	void doHttpRequest();

};


} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* CLIENTHTTPSYNC_H_ */
