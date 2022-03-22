/*
 * DemonizeWebSrv.h
 *
 *  Created on: Apr 6, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef DEMONIZEWEBSRV_H_
#define DEMONIZEWEBSRV_H_

#include <softel/core/system/BDemonizeBase.h>

namespace SF {
namespace WEB {

class DemonizeWebSrv: public SF::CORE::SYSTEM::BDemonizeBase {
public:
	DemonizeWebSrv(const std::string& address, const std::string& port,
			const std::string& threads, const std::string& rootDoc);
	virtual ~DemonizeWebSrv();

	void runDemonizedServer();

private:
	std::string m_address;
	std::string m_port;
	std::string m_rootDoc;
	std::size_t m_threads;
};

} /* namespace WEB */
} /* namespace SF */
#endif /* DEMONIZEWEBSRV_H_ */
