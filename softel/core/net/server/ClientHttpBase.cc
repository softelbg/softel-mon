/*
 * ClientHttpBase.cc
 *
 *  Created on: Nov 22, 2013
 *      Author: stan
 */

#include "ClientHttpBase.h"
#include <string>

namespace SF {
namespace CORE {
namespace NET {

ClientHttpBase::ClientHttpBase(const std::string& hostName, const std::string& hostPort,
		const std::string& urlpath, boost::asio::io_service& iosvc) :
		ClientBase(hostName, hostPort, iosvc),
		m_url_path(urlpath) {
	// TODO Auto-generated constructor stub

}

ClientHttpBase::~ClientHttpBase() {
	// TODO Auto-generated destructor stub
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
