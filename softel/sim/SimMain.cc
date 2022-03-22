/*
 * SimMain.cc
 *
 *  Created on: 27.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "SimMain.h"
#include "SimTester.h"
#include <softel/core/pool/WorkItemBase.h>
#include <softel/db/WorkItemMySqlQuery.h>
#include <softel/core/system/CSystemTools.h>
#include <softel/core/util/CTimeCounter.h>
#include <softel/mon/event/RQueueEvent.h>
#include <softel/mon/event/EventManager.h>
#include <softel/mon/event/EventHandlerManager.h>
#include <softel/mon/event/EventHandlerTest.h>
#include <softel/mon/event/EventQueueProvider.h>

#include <softel/mon/event/WorkItemEventProvider.h>

#include <softel/core/net/server/ServerHttpCmdBase.h>
#include <softel/core/net/server/ServerTcpBase.h>
#include <softel/core/net/web/ServerHttpWeb.h>
#include <softel/core/net/server/WorkItemServerTcp.h>

#include <softel/sim/SimFileIni.h>
#include <softel/sim/SimDbPool.h>

#include <softel/core/net/web/BPluginBase.h>
#include <softel/core/net/web/PluginContainerWebSrvSingleton.h>
#include <softel/sim/PluginSimTest.h>
#include <softel/sim/PluginSimInfo.h>

#include <softel/sim/SimSrvCommon.h>

#include <memory>
#include <boost/lexical_cast.hpp>

using namespace SF::CORE::SYSTEM;
using namespace SF::CORE::NET;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::POOL;
using namespace SF::CORE;
using namespace SF::MON::EVENT;
using namespace SF::DB;
using namespace SF::WEB;
using namespace std;

namespace SF {
namespace SIM {

SimMain::SimMain()
	: m_pThrPool(NULL) {
	// TODO Auto-generated constructor stub

}

SimMain::~SimMain() {
	// TODO Auto-generated destructor stub
}

void SimMain::start() {

	/** Init sim ini file manager*/
	SimFileIni::init();

	startThreadPool();
	startDbPool();

	/** Initialize Event Manager */
	m_event_manager.init();

	startWebSrv();

	// Start scheduled tasks
	startScheduled();

	// Start SIM Tests TODO: For debugging purposes only...
	SimTester st(m_event_manager);
	st.run();
}

/**
 * Init and start various scheduled tasks...
 */
void SimMain::startScheduled() {

	// DB refresh
	Ptr<WorkItemBase> wiSchedule(new WorkItemSchedule());
	Ptr<ScheduledItem> s1(new DbScheduledItem());
	s1->setPeriod(180);
	s1->setId("DbScheduledItem");
	dynamic_pointer_cast<WorkItemSchedule>(wiSchedule)->add(s1);
	m_pThrPool->postWorkItem(wiSchedule);


}

/**
 * Initialize the DB connection Pool.
 * This should be the default simd database (read from simd.ini)
 */
void SimMain::startDbPool() {
	SimDbPool::initSimPool(DbPool::MySql, 5);
	SimDbPool::getInstance()->init();
}

/**
 * Initialize Main Thread Pool
 */
void SimMain::startThreadPool() {
	int numThreads = boost::lexical_cast<int>(SimFileIni::getInstance()->get("SIM", "threadpoolsize"));
	SF::CORE::POOL::ThreadPoolAsioSingleton::init(numThreads);
	m_pThrPool = SF::CORE::POOL::ThreadPoolAsioSingleton::getInstance();
}

void SimMain::startWebSrv() {

	PluginContainerWebSrvSingleton::init();

	// Register SimTest Plugin
	Ptr<BPluginBase> plugin(new PluginSimTest());
	PluginContainerWebSrvSingleton::getInstance()->push_back(plugin->getKey(), plugin);

	// Register SimInfo plugin - various components info
	Ptr<BPluginBase> plugin2(new PluginSimInfo());
	dynamic_pointer_cast<PluginSimInfo>(plugin2)->subscribe(
			static_pointer_cast<IObjectInterface>(m_pThrPool));
	dynamic_pointer_cast<PluginSimInfo>(plugin2)->subscribe(
			dynamic_pointer_cast<IObjectInterface>(SimDbPool::getInstance()));
	PluginContainerWebSrvSingleton::getInstance()->push_back(plugin2->getKey(), plugin2);

	//Ptr<ServerTcpBase> srvHttpCmd(new ServerHttpCmdBase("0.0.0.0", "8080", 3));
	int numThreadsWebSrv =
			boost::lexical_cast<int>(SimFileIni::getInstance()->get("WebSrv", "threads"));
	Ptr<ServerTcpBase> srvHttpCmd(
			new SF::WEB::ServerHttpWeb(
					SimFileIni::getInstance()->get("WebSrv", "ip"),
					SimFileIni::getInstance()->get("WebSrv", "port"),
					SimFileIni::getInstance()->get("WebSrv", "docroot"),
					numThreadsWebSrv));
	Ptr<WorkItemBase> wiSrvHttp(new WorkItemServerTcp(srvHttpCmd));
	m_pThrPool->postWorkItem(wiSrvHttp);

}

} /* namespace SIM */
} /* namespace SF */
