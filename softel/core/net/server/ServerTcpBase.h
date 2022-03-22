/*
 * ServerTcpBase.h
 *
 *  Created on: Dec 2, 2013
 *      Author: sgeorgiev
 */

#ifndef SERVERTCPBASE_H_
#define SERVERTCPBASE_H_

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "ConnectionTcpBase.h"


namespace SF {
namespace CORE {
namespace NET {

class ServerTcpBase {
public:
	explicit ServerTcpBase(const std::string& address, const std::string& port,
				std::size_t thread_pool_size);
	virtual ~ServerTcpBase();

	// Run the server's io_service loop.
	void run();

	void start();
	void stop();

protected:
	virtual void startAccept();
	virtual void handleAccept(const boost::system::error_code& e);
	void handleStop();

	// The number of threads that will call io_service::run().
	std::size_t m_thread_pool_size;

	boost::asio::io_service m_io_service;

	/// The signal_set is used to register for process termination notifications.
	boost::asio::signal_set m_signals;

	// Acceptor used to listen for incoming connections.
	boost::asio::ip::tcp::acceptor m_acceptor;

	// The next connection to be accepted.
	boost::shared_ptr<ConnectionTcpBase> m_new_connection;

};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* SERVERTCPBASE_H_ */
