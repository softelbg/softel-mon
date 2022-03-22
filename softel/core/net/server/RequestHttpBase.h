/*
 * RequestHttpBase.h
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#ifndef REQUESTHTTPBASE_H_
#define REQUESTHTTPBASE_H_

#include <string>
#include <vector>
#include <map>
#include "RequestBase.h"
#include "ReplyHttpBase.h"

namespace SF {
namespace CORE {
namespace NET {

class RequestHttpBase: public RequestBase {
public:
	RequestHttpBase();
	virtual ~RequestHttpBase();

	virtual void postParse();

	virtual std::string toString() const;

	bool getHeaderValue(const std::string& key, std::string& value) const;

	// TODO: Hide these....
	std::string raw_request;
	std::string method;
	std::string uri;
	std::string host_based_uri_prefix;
	std::string uri_arg;
	std::string uri_file;
	int http_version_major;
	int http_version_minor;

	std::vector<header> headers;

	std::vector<std::string> m_token_arguments;
	std::map<std::string, std::string> m_arguments;

private:
	void doArguments();
	void buildHeadersIndex();
	void buildUriPrefix();

	std::map<std::string, size_t> idx_headers;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* REQUESTHTTPBASE_H_ */
