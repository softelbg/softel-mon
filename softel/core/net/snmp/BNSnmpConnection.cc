/*
 * BNSnmpConnection.cc
 *
 *  Created on: 05.10.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BNSnmpConnection.h"

using namespace std;

namespace SF {
namespace CORE {
namespace NET {

BNSnmpConnection::~BNSnmpConnection() {
	// TODO Auto-generated destructor stub
}

void BNSnmpConnection::connect(
		long version, const string& peername, const string& community) {

    struct snmp_session sess;
    snmp_sess_init(&sess);   /* initialize session */
    sess.version = SNMP_VERSION_2c;
    sess.peername = strdup(peername.c_str());
    sess.community = reinterpret_cast<u_char*>(strdup(community.c_str()));
    sess.community_len = community.size();
    sess.callback = asynch_response;
    snmp_handle_ = snmp_sess_open(&sess);
    free(sess.peername);
    free(sess.community);
    netsnmp_transport *transport  = snmp_sess_transport( snmp_handle_ );
    m_snmp_socket.assign(boost::asio::ip::udp::v4(), transport->sock);
    // Put the socket into non-blocking mode.
    udp::socket::non_blocking_io non_blocking_io(true);
    m_snmp_socket.io_control(non_blocking_io);
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
