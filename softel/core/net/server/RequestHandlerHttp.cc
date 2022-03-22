/*
 * RequestHandlerHttp.cc
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#include "RequestHandlerHttp.h"
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

RequestHandlerHttp::RequestHandlerHttp(const std::string& doc_root) :
		m_doc_root(doc_root) {

}

RequestHandlerHttp::~RequestHandlerHttp() {
	// TODO Auto-generated destructor stub
}

void RequestHandlerHttp::handleRequest(const boost::shared_ptr<RequestBase>& req, boost::shared_ptr<ReplyBase>& rep) {
	// Decode url to path.
	std::string request_path;
	if (!ProtoUtils::urlDecode(boost::static_pointer_cast<RequestHttpBase>(req)->uri, request_path)) {
		rep = ReplyHttpBase::stockReply(ReplyHttpBase::bad_request);
		return;
	}

	// Request path must be absolute and not contain "..".
	if (request_path.empty() || request_path[0] != '/'
			|| request_path.find("..") != std::string::npos) {
		rep = ReplyHttpBase::stockReply(ReplyHttpBase::bad_request);
		return;
	}

	// If path ends in slash (i.e. is a directory) then add "index.html".
	if (request_path[request_path.size() - 1] == '/') {
		request_path += "index.html";
	}

	// Determine the file extension.
	std::size_t last_slash_pos = request_path.find_last_of("/");
	std::size_t last_dot_pos = request_path.find_last_of(".");
	std::string extension;
	if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
		extension = request_path.substr(last_dot_pos + 1);
	}

	// Open the file to send back.
	std::string full_path = m_doc_root + request_path;
	std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
	if (!is) {
		rep = ReplyHttpBase::stockReply(ReplyHttpBase::not_found);
		return;
	}

	// Fill out the reply to be sent to the client.
	boost::shared_ptr<ReplyHttpBase> httpRep(new ReplyHttpBase());
	rep = httpRep;
	httpRep->status = ReplyHttpBase::ok;
	char buf[512];
	while (is.read(buf, sizeof(buf)).gcount() > 0)
		httpRep->content.append(buf, is.gcount());
	httpRep->headers.resize(2);
	httpRep->headers[0].name = "Content-Length";
	httpRep->headers[0].value = boost::lexical_cast < std::string
			> (httpRep->content.size());
	httpRep->headers[1].name = "Content-Type";
	httpRep->headers[1].value = MimeTypes::getMimeType(extension);
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

