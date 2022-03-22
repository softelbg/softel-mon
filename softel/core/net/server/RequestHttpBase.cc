/*
 * RequestHttpBase.cc
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#include "RequestHttpBase.h"
#include <softel/core/util/CStringUtil.h>

#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;
using namespace SF::CORE::UTIL;

namespace SF {
namespace CORE {
namespace NET {

RequestHttpBase::RequestHttpBase() {
	// TODO Auto-generated constructor stub

}

RequestHttpBase::~RequestHttpBase() {
	// TODO Auto-generated destructor stub
}

/**
 * Post parse - should be launched after initial parsing during receive
 *
 * Should parse URI and provide the arguments.
 *
 */
void RequestHttpBase::postParse() {

	uri_file = uri;

	// dispatch http methods
	if (method.compare("GET") == 0) {

		size_t pos = uri.find_first_of('?', 0);

		if (pos <= uri.size()) {
			uri_arg = uri.substr(pos + 1);
			uri_file = uri.substr(0, pos);
			doArguments();
		}

		m_request = uri;

	} else if (method.compare("POST") == 0) {

		size_t pos = raw_request.find("\r\n\r\n", 0);

		if (pos != string::npos) {
			uri_arg = raw_request.substr(pos + 4);
			doArguments();
		}

		m_request = uri;
		m_request += (uri_arg.empty()) ? "" : "?" + uri_arg;
		uri = m_request;
	}

	buildHeadersIndex();

	buildUriPrefix();

}

/**
 * Build URI Prefix based on the Host header option.
 *
 * For example sub1.softel.bg/test/test.html should finally be aquired from
 *  /base/www/path/bg/softel/sub1/test/test.html
 *
 *  sub1.softel.bg/test/test.html => /base/www/path/bg/softel/sub1/test/test.html
 *
 */
void RequestHttpBase::buildUriPrefix() {

	vector < string > tokenizedServer;
	string headerServer;

	if (getHeaderValue("Host", headerServer)) {

		// Remove port if any
		size_t pos = headerServer.find(':');
		if (pos <= headerServer.size()) {
			headerServer = headerServer.substr(0, pos);
		}

		CStringUtil::tokenize(headerServer, ".", tokenizedServer);

		// Implode in reverse order
		reverse(tokenizedServer.begin(),tokenizedServer.end());
		CStringUtil::implode(tokenizedServer, "/", host_based_uri_prefix);

		// TODO: Not sure is it needed but for any case...
		host_based_uri_prefix.push_back('/');
		host_based_uri_prefix.insert(0, 1, '/');

		cout << __FUNCTION__ << " server_based_uri_prefix: " << host_based_uri_prefix << endl;
	}
}

void RequestHttpBase::buildHeadersIndex() {
	// build headers index
	for (size_t i = 0; i < headers.size(); i++) {
		idx_headers[headers[i].name] = i;
	}
}

/**
 * Parse raw arguments and produce argument lists
 */
void RequestHttpBase::doArguments() {

	CStringUtil::remove('?', uri_arg);
	CStringUtil::tokenize(uri_arg, "&", m_token_arguments);

	for (size_t i = 0; i < m_token_arguments.size(); i++) {
		vector < string > tokenKeyValue;
		CStringUtil::tokenize(m_token_arguments[i], "=", tokenKeyValue);
		if (tokenKeyValue.size() == 2) {
			m_arguments[tokenKeyValue[0]] = tokenKeyValue[1];
		}
	}
}

std::string RequestHttpBase::toString() const {

	stringstream ss;

	ss << endl << __FILE__ << "::" << __FUNCTION__ << endl;
	ss << "Method[" << method << "]" << endl;
	ss << "URI[" << uri << "] " << endl;
	ss << "uri_arg[" << uri_arg << "]" << endl;
	ss << "raw_request[" << raw_request << "] " << endl;
	ss << "m_request[" << m_request << "] " << endl;
	ss << "host_based_uri_prefix[" << host_based_uri_prefix << "] " << endl;
	ss << "uri_file[" << uri_file << "]" << endl;

	for (size_t i = 0; i < headers.size(); i++) {
		ss << "header[" << i << "] " << headers[i].name << " = "
				<< headers[i].value << endl;
	}

	ss << endl << "m_arguments" << endl;
	for(map<string, string>::const_iterator it = m_arguments.begin(); it != m_arguments.end(); it++) {
		ss << "   " << it->first << " = " << it->second << endl;
	}

	return ss.str();
}

bool RequestHttpBase::getHeaderValue(const string& key, std::string& value) const {
	map<string, size_t>::const_iterator it;
	it = idx_headers.find(key);

	if (it != idx_headers.end()) {
		value = headers[it->second].value;
		return true;
	} else {
		return false;
	}
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
