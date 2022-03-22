/*
 * RequestHandlerHttpCmdBase.h
 *
 *  Created on: Dec 6, 2013
 *      Author: sgeorgiev
 */

#ifndef REQUESTHANDLERHTTPCMDBASE_H_
#define REQUESTHANDLERHTTPCMDBASE_H_

#include "RequestHandlerBase.h"

namespace SF {
namespace CORE {
namespace NET {

class RequestHandlerHttpCmdBase: public SF::CORE::NET::RequestHandlerBase {
public:
	RequestHandlerHttpCmdBase();
	RequestHandlerHttpCmdBase(bool has_reply_header);
	virtual ~RequestHandlerHttpCmdBase();

	virtual void handleRequest(const boost::shared_ptr<RequestBase>& req, boost::shared_ptr<ReplyBase>& rep);

private:
	bool m_has_reply_header;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* REQUESTHANDLERHTTPCMDBASE_H_ */
