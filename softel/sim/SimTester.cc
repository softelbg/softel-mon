/*
 * SimTester.cc
 *
 *  Created on: Dec 12, 2013
 *      Author: stan
 */

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
#include <softel/core/net/server/WorkItemServerTcp.h>

#include <softel/core/pool/DbPool.h>

#include <memory>

using namespace SF::CORE::SYSTEM;
using namespace SF::CORE::NET;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::POOL;
using namespace SF::CORE;
using namespace SF::MON::EVENT;
using namespace SF::DB;
using namespace std;

namespace SF {
namespace SIM {

SimTester::SimTester(SF::MON::EVENT::EventManager& event_manager)
	: m_event_manager(event_manager) {

	cout << __FILE__ << "::" << __FUNCTION__ << endl;

	m_pThrPool = SF::CORE::POOL::ThreadPoolAsioSingleton::getInstance();

}

SimTester::~SimTester() {
	// TODO Auto-generated destructor stub
}

void SimTester::run() {

	cout << __FILE__ << "::" << __FUNCTION__ << endl;

	//startSqlTest();
	//startEventManagerTest();

	startTestDbPool();
}

void SimTester::startTestDbPool() {

	DbPool dbPool(DbPool::MySql, 10);
	dbPool.init();

	boost::atomic<int> active(0);
	boost::atomic<int> failed(0);

	Ptr<IDbPostCallBackInterface> cb(new DbCallBack(active, failed));

	DbResult r;

	dbPool.execute("DROP TABLE SIM_TEST", r);
	cout << __FILE__ << "::" << __FUNCTION__ << r.toString() << endl;
	dbPool.execute("CREATE TABLE SIM_TEST (id int, val int, PRIMARY KEY (id), KEY keyval (val))", r);
	cout << __FILE__ << "::" << __FUNCTION__ << r.toString() << endl;

	CTimeCounter cnt;

	for (int i = 1; i <= 1000; i++) {
		stringstream ss;

		ss << "INSERT INTO SIM_TEST " << "(id, val) VALUES (" << i << ", " << i * 3 << ")";

		dbPool.post(ss.str(), cb);
	}

	while(true) {
		cout << __FILE__ << "::" << __FUNCTION__ << " Waiting active " << active << endl;
		CSystemTools::doSleep(1);

		dbPool.doSelect("SELECT count(*) FROM SIM_TEST", r);
		cout << __FILE__ << "::" << __FUNCTION__ << r.toString() << endl;

		if (active == 0) {
			break;
		}
	}

	cnt.stop();

	cout << "Finished " << __FILE__ << "::" << __FUNCTION__ << " " << cnt.getElapsedFormated();
	cout << " active: " << active << " failed: " << failed << endl;

	dbPool.doSelect("SELECT count(*) FROM SIM_TEST", r);
	cout << __FILE__ << "::" << __FUNCTION__ << r.toString() << endl;
}

/**
 * Post an sql query for execution.
 *
 * TODO: Make a proper db pool for sync/async sql queries.
 *
 */
void SimTester::postSql(const string& sql, const int id, ThreadPoolAsio& thPool) {
	SF::CORE::Ptr<WorkItemBase> ptrSqlWI(new WorkItemMySqlQuery(sql));
	ptrSqlWI->setParam("id", id);
	thPool.postWorkItem(ptrSqlWI);
}

void SimTester::startSqlTest() {

	cout << __FILE__ << "::" << __FUNCTION__ << endl;

	ThreadPoolAsio thPool(10);

	CTimeCounter cnt;

	int j = 0;

	postSql("USE dbtest", j++, thPool);
	postSql("CREATE TABLE SIM_TEST (id int, val int)", j++, thPool);
	postSql("DELETE FROM SIM_TEST", j++, thPool);

	while (thPool.getActive() > 0) {
		CSystemTools::doSleep(1);
	}

	for (int i = 1; i <= 10000; i++) {
		stringstream ss;

		ss << "INSERT INTO SIM_TEST " << "(id, val) VALUES (" << i << ", " << i * 3 << ")";

		postSql(ss.str(), j++, thPool);
	}

	while (thPool.getActive() > 0) {
		thPool.printStatus();
		CSystemTools::doSleep(1);
	}

	cnt.stop();

	cout << "Finished " << __FILE__ << "::" << __FUNCTION__ << " "
			<< cnt.getElapsedFormated() << endl;

	CSystemTools::doSleep(600);
}

void SimTester::startEventManagerTest() {

	CTimeCounter cnt;

	for (int i = 0; i < 10; i++) {
		SF::CORE::Ptr<SF::MON::EVENT::EventHandlerBase> eventHandler(
				new SF::MON::EVENT::EventHandlerTest(i));
		m_event_manager.addHandler(eventHandler);
	}

	for (int i = 1; i <= 10; i++) {
		SF::CORE::Ptr<WorkItemBase> ptrWIP(
				new WorkItemEventProvider(i, m_event_manager));
		m_pThrPool->postWorkItem(ptrWIP);
	}

	while (m_pThrPool->getActive() > 0) {
		m_pThrPool->printStatus();
		cout << "sizeEventQueue: " << m_event_manager.sizeEventQueue() << endl;

		EventQueueProvider& eventQueue =
				m_event_manager.getEventQueueProvider();
		cout << "sizeAllEventsQueue: " << eventQueue.size() << endl;

		CSystemTools::doSleep(1);
	}

	cnt.stop();
	cout << "Finished " << __FILE__ << "::" << __FUNCTION__ << " "
			<< cnt.getElapsedFormated() << endl;
	CSystemTools::doSleep(600);
}

DbCallBack::DbCallBack(boost::atomic<int>& active, boost::atomic<int>& failed) :
		m_active(active), m_failed(failed) {

}

void DbCallBack::onPost() {
	m_active++;
}

void DbCallBack::onError(DbResult& result) {
	m_active--;
	m_failed++;
	DbPoolPostCallBackBase::onError(result);
}

void DbCallBack::onFinished(DbResult& result) {
	m_active--;
	DbPoolPostCallBackBase::onFinished(result);
}

} /* namespace SIM */
} /* namespace SF */


