/*
 * RequestHandlerBase.h
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#ifndef REQUESTHANDLERBASE_H_
#define REQUESTHANDLERBASE_H_

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include "IRequestHandlerInterface.h"
#include "RequestBase.h"
#include "ReplyBase.h"

namespace SF {
namespace CORE {
namespace NET {

struct reply;
struct request;

class RequestHandlerBase : virtual public IRequestHandlerInterface, private boost::noncopyable {
public:
	RequestHandlerBase();
	virtual ~RequestHandlerBase();

	virtual void handleRequest(const boost::shared_ptr<RequestBase>& req, boost::shared_ptr<ReplyBase>& rep);
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* REQUESTHANDLERBASE_H_ */
