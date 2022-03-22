/*
 * RequestHandlerBase.cc
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#include "RequestHandlerBase.h"
#include "ReplyBase.h"
#include "ReplyHttpBase.h"


namespace SF {
namespace CORE {
namespace NET {

RequestHandlerBase::~RequestHandlerBase() {
	// TODO Auto-generated destructor stub
}

RequestHandlerBase::RequestHandlerBase() {
	// TODO Auto-generated constructor stub

}

void RequestHandlerBase::handleRequest(const boost::shared_ptr<RequestBase>& req, boost::shared_ptr<ReplyBase>& rep) {
	rep = ReplyHttpBase::stockReply(ReplyHttpBase::moved_temporarily);
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
