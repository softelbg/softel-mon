/*
 * PluginContainerWebSrvSingleton.h
 *
 *  Created on: Apr 12, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef PLUGINCONTAINERWEBSRVSINGLETON_H_
#define PLUGINCONTAINERWEBSRVSINGLETON_H_

#include "PluginContainer.h"
#include <softel/core/lock/BoostSynchronized.h>
#include <softel/core/Ptr.h>

namespace SF {
namespace WEB {

class PluginContainerWebSrvSingleton: public PluginContainer {
public:

	virtual ~PluginContainerWebSrvSingleton();

	static void init();
	static SF::CORE::Ptr<PluginContainerWebSrvSingleton> getInstance();

private:
	PluginContainerWebSrvSingleton();

	static SF::CORE::Ptr<PluginContainerWebSrvSingleton> m_pInstance;
};

} /* namespace WEB */
} /* namespace SF */
#endif /* PLUGINCONTAINERWEBSRVSINGLETON_H_ */
