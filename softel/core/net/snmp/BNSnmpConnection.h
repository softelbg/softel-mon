/*
 * BNSnmpConnection.h
 *
 *	Boost ASIO - Net-SNMP wrapper...
 *
 *  Created on: 05.10.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BNSNMPCONNECTION_H_
#define BNSNMPCONNECTION_H_

#include <softel/core/CObject.h>
#include <string>

namespace SF {
namespace CORE {
namespace NET {

/*
 *
 */
class BNSnmpConnection: public SF::CORE::CObject {
public:
public:
	BNSnmpConnection(boost::asio::io_service& io_service)
	: snmp_socket_(io_service),
	m_snmp_handle(NULL) {};
	~BNSnmpConnection() {
		snmp_sess_close(this->m_snmp_handle);
	}
	void connect(long version,
			const std::string& peername, const std::string& community);
	void async_snmp_get(const std::string& snmp_oid, SNMPHandler handler);
	void async_snmp_set(const std::string& snmp_oid, const int type,
			const std::string& value, SNMPHandler handler);
private:
	udp::socket m_snmp_socket;
	void* m_snmp_handle;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* BNSNMPCONNECTION_H_ */
