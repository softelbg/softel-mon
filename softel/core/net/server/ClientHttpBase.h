/*
 * ClientHttpBase.h
 *
 *  Created on: Nov 22, 2013
 *      Author: stan
 */

#ifndef CLIENTHTTPBASE_H_
#define CLIENTHTTPBASE_H_

#include "ClientBase.h"

namespace SF {
namespace CORE {
namespace NET {

class ClientHttpBase: public SF::CORE::NET::ClientBase {
public:
	ClientHttpBase(const std::string& hostName, const std::string& hostPort,
			const std::string& urlpath, boost::asio::io_service& iosvc);
	virtual ~ClientHttpBase();

protected:
	std::string m_url_path;

};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* CLIENTHTTPBASE_H_ */
