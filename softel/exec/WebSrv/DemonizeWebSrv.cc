/*
 * DemonizeWebSrv.cc
 *
 *  Created on: Apr 6, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "DemonizeWebSrv.h"
#include <softel/core/net/web/ServerHttpWeb.h>

#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace SF {
namespace WEB {

DemonizeWebSrv::DemonizeWebSrv(const string& address, const string& port,
		const string& threads, const string& rootDoc) :
	m_address(address), m_port(port), m_rootDoc(rootDoc) {

	m_threads = boost::lexical_cast<std::size_t>(threads);
}

DemonizeWebSrv::~DemonizeWebSrv() {
	// TODO Auto-generated destructor stub
}

void DemonizeWebSrv::runDemonizedServer() {

	cout << __FILE__ << "::" << __FUNCTION__ << " m_address:" << m_address;
	cout << " m_port:" << m_port << " m_rootDoc:" << m_rootDoc;
	cout << " m_threads:"<< m_threads << endl;

	ServerHttpWeb s(m_address, m_port, m_rootDoc, m_threads);
	s.run();
}

} /* namespace WEB */
} /* namespace SF */
