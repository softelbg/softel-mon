/*
 * RequestBase.h
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#ifndef REQUESTBASE_H_
#define REQUESTBASE_H_

#include <softel/core/CObject.h>

#include <boost/asio/ip/address.hpp>
#include <string>

namespace SF {
namespace CORE {
namespace NET {

class RequestBase : public SF::CORE::CObject {
public:

	RequestBase();
	virtual ~RequestBase();

	virtual void postParse() {}

	const std::string& getRequest() const {
		return m_request;
	}

	void setRequest(const std::string& request) {
		m_request = request;
	}

	virtual std::string toString() const {
		return std::string("RequestBase: ") + m_request;
	}

	void setRemoteIp(const std::string& ip) {
		m_remote_ip = ip;
	}

	const std::string& getRemoteIp() const {
		return m_remote_ip;
	}

protected:
	std::string m_request;

	// TODO: Consider using boost address...
	std::string m_remote_ip;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* REQUESTBASE_H_ */
