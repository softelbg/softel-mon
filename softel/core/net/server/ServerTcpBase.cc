/*
 * ServerTcpBase.cc
 *
 *  Created on: Dec 2, 2013
 *      Author: sgeorgiev
 */

#include "ServerTcpBase.h"
#include <softel/core/system/CSystemTools.h>

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <iostream>

namespace SF {
namespace CORE {
namespace NET {

ServerTcpBase::~ServerTcpBase() {
	// TODO Auto-generated destructor stub
}


ServerTcpBase::ServerTcpBase(const std::string& address, const std::string& port,
    std::size_t thread_pool_size)
  : m_thread_pool_size(thread_pool_size),
    m_signals(m_io_service),
    m_acceptor(m_io_service),
    m_new_connection()
{
	// Register to handle the signals that indicate when the server should exit.
	// It is safe to register for the same signal multiple times in a program,
	// provided all registration for the specified signal is made through Asio.
/*
	m_signals.add(SIGINT);
	m_signals.add(SIGTERM);
#if defined(SIGQUIT)
	m_signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
	m_signals.async_wait(boost::bind(&ServerTcpBase::handleStop, this));
*/

	std::cout << __FILE__ << "::" << __FUNCTION__ << " address:" << address;
	std::cout << " port:" << port << " m_thread_pool_size:" << m_thread_pool_size << std::endl;

	// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
	boost::asio::ip::tcp::resolver resolver(m_io_service);
	boost::asio::ip::tcp::resolver::query query(address, port);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
	m_acceptor.open(endpoint.protocol());
	m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	m_acceptor.bind(endpoint);
	m_acceptor.listen();

	std::cout << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << std::endl;
}

void ServerTcpBase::run() {

	std::cout << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << std::endl;

	startAccept();

	// Create a pool of threads to run all of the io_services.
	std::vector<boost::shared_ptr<boost::thread> > threads;
	for (std::size_t i = 0; i < m_thread_pool_size; ++i) {
		boost::shared_ptr<boost::thread> thread(new boost::thread(
				boost::bind(&boost::asio::io_service::run, &m_io_service)));
		threads.push_back(thread);
    }

	std::cout << __FILE__ << "::" << __FUNCTION__ << " Created " << m_thread_pool_size << " threads\n";

	// TODO: There is a problem with join here ...
	// Wait for all threads in the pool to exit.
	/*
	for (std::size_t i = 0; i < threads.size(); ++i) {
		std::cout << __FILE__ << "::" << __FUNCTION__ << " Joining thread " << i << std::endl;
		threads[i]->join();
	}
	 */

	// TODO: Handle this Spin Lock here...
	while (true) {
		SF::CORE::SYSTEM::CSystemTools::doSleep(3600);
	}

	std::cout << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << std::endl;
}

void ServerTcpBase::startAccept() {
	m_new_connection.reset(new ConnectionTcpBase(m_io_service));
	m_acceptor.async_accept(m_new_connection->getSocket(),
			boost::bind(&ServerTcpBase::handleAccept, this,
					boost::asio::placeholders::error));
}

void ServerTcpBase::handleAccept(const boost::system::error_code& e) {

	if (!e) {
		m_new_connection->start();
	}

	startAccept();
}

void ServerTcpBase::handleStop() {
	m_io_service.stop();
}

void ServerTcpBase::start() {
	run();
}

void ServerTcpBase::stop() {
	handleStop();
}


} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
