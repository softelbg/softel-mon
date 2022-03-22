/*
 * WorkItemServerTcp.h
 *
 *  Created on: Dec 12, 2013
 *      Author: stan
 */

#ifndef WORKITEMSERVERTCP_H_
#define WORKITEMSERVERTCP_H_

#include <softel/core/pool/WorkItemBase.h>
#include <softel/core/Ptr.h>
#include "ServerTcpBase.h"

namespace SF {
namespace CORE {
namespace NET {

class WorkItemServerTcp: public SF::CORE::POOL::WorkItemBase {
public:
	WorkItemServerTcp(const SF::CORE::Ptr<ServerTcpBase>& tcp_server);
	virtual ~WorkItemServerTcp();

	void run();

	void stop();

private:
	SF::CORE::Ptr<ServerTcpBase> m_tcp_server;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* WORKITEMSERVERTCP_H_ */
