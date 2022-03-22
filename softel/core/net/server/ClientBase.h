/*
 * ClientBase.h
 *
 *  Created on: Nov 22, 2013
 *      Author: stan
 */

#ifndef CLIENTBASE_H_
#define CLIENTBASE_H_

#include <softel/core/CObject.h>
#include <softel/core/Ptr.h>
#include "Host.h"
#include <boost/asio.hpp>

namespace SF {
namespace CORE {
namespace NET {

class ClientBase: public SF::CORE::CObject {
public:
	ClientBase(const std::string& hostName, const std::string& hostPort, boost::asio::io_service& iosvc);
	virtual ~ClientBase();

	virtual void connect();

protected:
	boost::asio::io_service& m_io_service;

	boost::asio::ip::tcp::socket m_socket;

	boost::asio::streambuf m_request;
	boost::asio::streambuf m_response;

	Host m_host;

private:

};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* CLIENTBASE_H_ */
