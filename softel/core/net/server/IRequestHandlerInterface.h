/*
 * IRequestHandlerInterface.h
 *
 *  Created on: Dec 6, 2013
 *      Author: sgeorgiev
 */

#ifndef IREQUESTHANDLERINTERFACE_H_
#define IREQUESTHANDLERINTERFACE_H_

#include <boost/shared_ptr.hpp>

#include "RequestBase.h"
#include "ReplyBase.h"

namespace SF {
namespace CORE {
namespace NET {

class IRequestHandlerInterface {

	virtual void handleRequest(const boost::shared_ptr<RequestBase>& req,
				boost::shared_ptr<ReplyBase>& rep) = 0;

};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* IREQUESTHANDLERINTERFACE_H_ */
