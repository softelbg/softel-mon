/*
 * WorkItemServerTcp.cc
 *
 *  Created on: Dec 12, 2013
 *      Author: stan
 */

#include "WorkItemServerTcp.h"

namespace SF {
namespace CORE {
namespace NET {

WorkItemServerTcp::WorkItemServerTcp(const SF::CORE::Ptr<ServerTcpBase>& tcp_server) {

	m_tcp_server = tcp_server;

}

WorkItemServerTcp::~WorkItemServerTcp() {
	// TODO Auto-generated destructor stub
}

void WorkItemServerTcp::run() {
	m_tcp_server->start();
}

void WorkItemServerTcp::stop() {
	m_tcp_server->stop();
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
