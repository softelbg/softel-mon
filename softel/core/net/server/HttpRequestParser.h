/*
 * HttpRequestParser.h
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#ifndef HTTPREQUESTPARSER_H_
#define HTTPREQUESTPARSER_H_

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>
#include "RequestHttpBase.h"
#include <boost/enable_shared_from_this.hpp>

namespace SF {
namespace CORE {
namespace NET {

class HttpRequestParser {
public:
	HttpRequestParser();
	virtual ~HttpRequestParser();

	/// Reset to initial parser state.
	void reset();

	/// Parse some data. The tribool return value is true when a complete request
	/// has been parsed, false if the data is invalid, indeterminate when more
	/// data is required. The InputIterator return value indicates how much of the
	/// input has been consumed.
	template<typename InputIterator>
	boost::tuple<boost::tribool, InputIterator> parse(boost::shared_ptr<RequestBase>& req,
			InputIterator begin, InputIterator end) {

		boost::shared_ptr<RequestHttpBase> httpRequest(boost::static_pointer_cast<RequestHttpBase>(req));

		httpRequest->raw_request.append(begin, end);

		while (begin != end) {
			boost::tribool result = consume(httpRequest, *begin++);
			if (result || !result)
				return boost::make_tuple(result, begin);
		}
		boost::tribool result = boost::indeterminate;
		return boost::make_tuple(result, begin);
	}

private:
	/// Handle the next character of input.
	boost::tribool consume(boost::shared_ptr<RequestHttpBase>& req, char input);

	/// Check if a byte is an HTTP character.
	static bool is_char(int c);

	/// Check if a byte is an HTTP control character.
	static bool is_ctl(int c);

	/// Check if a byte is defined as an HTTP tspecial character.
	static bool is_tspecial(int c);

	/// Check if a byte is a digit.
	static bool is_digit(int c);

	/// The current state of the parser.
	enum state {
		method_start,
		method,
		uri,
		http_version_h,
		http_version_t_1,
		http_version_t_2,
		http_version_p,
		http_version_slash,
		http_version_major_start,
		http_version_major,
		http_version_minor_start,
		http_version_minor,
		expecting_newline_1,
		header_line_start,
		header_lws,
		header_name,
		space_before_header_value,
		header_value,
		expecting_newline_2,
		expecting_newline_3
	} state_;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* HTTPREQUESTPARSER_H_ */
