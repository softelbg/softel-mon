/*
 * PluginContainerWebSrvSingleton.cc
 *
 *  Created on: Apr 12, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "PluginContainerWebSrvSingleton.h"
#include <cassert>

using namespace SF::CORE::LOCK;
using namespace SF::CORE::POOL;
using namespace SF::CORE;

namespace SF {
namespace WEB {

Ptr<PluginContainerWebSrvSingleton> PluginContainerWebSrvSingleton::m_pInstance;

PluginContainerWebSrvSingleton::PluginContainerWebSrvSingleton() {
	// TODO Auto-generated constructor stub

}

PluginContainerWebSrvSingleton::~PluginContainerWebSrvSingleton() {
	// TODO Auto-generated destructor stub
}

void PluginContainerWebSrvSingleton::init() {
	m_pInstance.reset(new PluginContainerWebSrvSingleton());
}

Ptr<PluginContainerWebSrvSingleton> PluginContainerWebSrvSingleton::getInstance() {
	assert(m_pInstance);
	return m_pInstance;
}

} /* namespace WEB */
} /* namespace SF */
