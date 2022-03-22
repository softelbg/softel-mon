/*
 * RequestHandlerHttp.h
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#ifndef REQUESTHANDLERHTTP_H_
#define REQUESTHANDLERHTTP_H_

#include <boost/shared_ptr.hpp>
#include "RequestHandlerBase.h"
#include "RequestBase.h"
#include "ReplyBase.h"

namespace SF {
namespace CORE {
namespace NET {

class RequestHandlerHttp : public RequestHandlerBase {
public:
	explicit RequestHandlerHttp(const std::string& doc_root);
	virtual ~RequestHandlerHttp();

	virtual void handleRequest(const boost::shared_ptr<RequestBase>& req, boost::shared_ptr<ReplyBase>& rep);

private:
	std::string m_doc_root;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* REQUESTHANDLERHTTP_H_ */
