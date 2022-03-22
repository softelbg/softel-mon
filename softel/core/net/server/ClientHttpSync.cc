/*
 * ClientHttpSync.cc
 *
 *  Created on: Nov 22, 2013
 *      Author: stan
 */

#include "ClientHttpSync.h"
#include <softel/core/error/CErrorCommon.h>

using namespace std;
using namespace SF::CORE::ERROR;

namespace SF {
namespace CORE {
namespace NET {

ClientHttpSync::ClientHttpSync(const std::string& hostName, const std::string& hostPort,
		const std::string& urlpath, boost::asio::io_service& iosvc) :
	ClientHttpBase(hostName, hostPort, urlpath, iosvc) {
	// TODO Auto-generated constructor stub

}

ClientHttpSync::~ClientHttpSync() {
	// TODO Auto-generated destructor stub
}

void ClientHttpSync::doHttpRequest() {

	std::ostream request_stream(&m_request);
	request_stream << "GET " << m_url_path << " HTTP/1.0\r\n";
	request_stream << "Host: " << m_host.getHostName() << "\r\n";
	request_stream << "Accept: */*\r\n";
	request_stream << "Connection: close\r\n\r\n";

	// Send the request.
	boost::asio::write(m_socket, m_request);

	// Read the response status line. The response streambuf will automatically
	// grow to accommodate the entire line. The growth may be limited by passing
	// a maximum size to the streambuf constructor.
	boost::asio::read_until(m_socket, m_response, "\r\n");

	// Check that response is OK.
	std::istream response_stream(&m_response);
	std::string http_version;
	response_stream >> http_version;
	unsigned int status_code;
	response_stream >> status_code;
	std::string status_message;
	std::getline(response_stream, status_message);
	if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
		throw CErrorCommon("Invalid response");
	}
	if (status_code != 200) {
		std::cout << "Response returned with status code " << status_code
				<< "\n";
		throw CErrorCommon("Response returned with status code "); // TODO Fix msg
	}

	// Read the response headers, which are terminated by a blank line.
	boost::asio::read_until(m_socket, m_response, "\r\n\r\n");

	// Process the response headers.
	std::string header;
	while (std::getline(response_stream, header) && header != "\r")
		std::cout << header << "\n";
	std::cout << "\n";

	// Write whatever content we already have to output.
	if (m_response.size() > 0)
		std::cout << &m_response;

	// Read until EOF, writing data to output as we go.
	boost::system::error_code error;
	while (boost::asio::read(m_socket, m_response,
			boost::asio::transfer_at_least(1), error))
		std::cout << &m_response;
	if (error != boost::asio::error::eof)
		throw boost::system::system_error(error);

}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
