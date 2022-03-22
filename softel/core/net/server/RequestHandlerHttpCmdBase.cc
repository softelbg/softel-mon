/*
 * RequestHandlerHttpCmdBase.cc
 *
 *  Created on: Dec 6, 2013
 *      Author: sgeorgiev
 */

#include "RequestHandlerHttpCmdBase.h"
#include "RequestHttpBase.h"
#include "RequestBase.h"
#include "ProtoUtils.h"
#include "ReplyHttpBase.h"

#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "MimeTypes.h"

namespace SF {
namespace CORE {
namespace NET {

RequestHandlerHttpCmdBase::RequestHandlerHttpCmdBase()
	: m_has_reply_header(true) {

}

RequestHandlerHttpCmdBase::RequestHandlerHttpCmdBase(bool has_reply_header)
	: m_has_reply_header(has_reply_header) {

}

RequestHandlerHttpCmdBase::~RequestHandlerHttpCmdBase() {
	// TODO Auto-generated destructor stub
}

void RequestHandlerHttpCmdBase::handleRequest(const boost::shared_ptr<RequestBase>& req, boost::shared_ptr<ReplyBase>& rep) {
	// Decode url to path.
	std::string request_path;
	if (!ProtoUtils::urlDecode(boost::static_pointer_cast<RequestHttpBase>(req)->uri, request_path)) {
		rep = ReplyHttpBase::stockReply(ReplyHttpBase::bad_request);
		return;
	}

	// Fill out the reply to be sent to the client.
	boost::shared_ptr<ReplyHttpBase> httpRep(new ReplyHttpBase());
	rep = httpRep;

	httpRep->status = ReplyHttpBase::ok;

	// TODO: Here should be parsed the request and prepare the reply
	httpRep->content = std::string("REQ: ") + request_path;

	if (m_has_reply_header) {
		httpRep->headers.resize(2);
		httpRep->headers[0].name = "Content-Length";
		httpRep->headers[0].value = boost::lexical_cast<std::string> (httpRep->content.size());
		httpRep->headers[1].name = "Content-Type";
		httpRep->headers[1].value = MimeTypes::getMimeType("");
	}

}



} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
