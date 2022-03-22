/*
 * ReplyHttpBase.h
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#ifndef REPLYHTTPBASE_H_
#define REPLYHTTPBASE_H_

#include <string>
#include <vector>
#include <boost/asio.hpp>

#include "ReplyBase.h"
#include <boost/shared_ptr.hpp>

namespace SF {
namespace CORE {
namespace NET {

struct header {
	std::string name;
	std::string value;
};

class ReplyHttpBase: public ReplyBase {
public:

	// The status of the reply.
	enum statusType {
		ok = 200,
		created = 201,
		accepted = 202,
		no_content = 204,
		multiple_choices = 300,
		moved_permanently = 301,
		moved_temporarily = 302,
		not_modified = 304,
		bad_request = 400,
		unauthorized = 401,
		forbidden = 403,
		not_found = 404,
		internal_server_error = 500,
		not_implemented = 501,
		bad_gateway = 502,
		service_unavailable = 503
	} status;

	// The headers to be included in the reply.
	std::vector<header> headers;

	// The content to be sent in the reply.
	std::string content;

	// Convert the reply into a vector of buffers. The buffers do not own the
	// underlying memory blocks, therefore the reply object must remain valid and
	// not be changed until the write operation has completed.
	virtual std::vector<boost::asio::const_buffer> toBuffers();

	// Get a stock reply.
	static boost::shared_ptr<ReplyHttpBase> stockReply(statusType status);

};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* REPLYHTTPBASE_H_ */
