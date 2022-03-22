/*
 * PluginSimTest.h
 *
 *  Created on: Apr 18, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef PLUGINSIMTEST_H_
#define PLUGINSIMTEST_H_

#include <softel/core/net/web/BPluginBase.h>

namespace SF {
namespace SIM {

class PluginSimTest: public SF::WEB::BPluginBase {
public:
	PluginSimTest();
	virtual ~PluginSimTest();

	virtual void run(const SF::CORE::NET::RequestHttpBase& req);

};

} /* namespace SIM */
} /* namespace SF */
#endif /* PLUGINSIMTEST_H_ */
