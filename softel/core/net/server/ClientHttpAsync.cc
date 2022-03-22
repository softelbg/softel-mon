/*
 * ClientHttpAsync.cc
 *
 *  Created on: Nov 22, 2013
 *      Author: stan
 */

#include "ClientHttpAsync.h"
#include <string>

namespace SF {
namespace CORE {
namespace NET {

ClientHttpAsync::ClientHttpAsync(const std::string& hostName, const std::string& hostPort,
		const std::string& urlpath, boost::asio::io_service& iosvc) :
		ClientHttpBase(hostName, hostPort, urlpath, iosvc) {
	// TODO Auto-generated constructor stub

}

ClientHttpAsync::~ClientHttpAsync() {
	// TODO Auto-generated destructor stub
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
