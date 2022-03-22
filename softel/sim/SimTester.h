/*
 * SimTester.h
 *
 *  Created on: Dec 12, 2013
 *      Author: stan
 */

#ifndef SIMTESTER_H_
#define SIMTESTER_H_

#include <softel/core/CObject.h>
#include <softel/core/intf/CRunnableInterface.h>
#include <softel/mon/event/EventManager.h>
#include <softel/core/commonDefs.h>
#include <boost/atomic.hpp>

namespace SF {
namespace SIM {

class DbCallBack : public SF::CORE::POOL::DbPoolPostCallBackBase {
public:
	DbCallBack(boost::atomic<int>& active, boost::atomic<int>& failed);
	virtual ~DbCallBack() {}

	virtual void onPost();
	virtual void onError(SF::CORE::POOL::DbResult& result);
	virtual void onFinished(SF::CORE::POOL::DbResult& result);

private:
	boost::atomic<int>& m_active;
	boost::atomic<int>& m_failed;
};

class SimTester: public SF::CORE::CObject,
		virtual SF::CORE::INTF::CRunnableInterface {
public:
	SimTester(SF::MON::EVENT::EventManager& event_manager);
	virtual ~SimTester();

	void run();

private:

	void startSqlTest();
	void postSql(const std::string& sql, const int id, SF::CORE::POOL::ThreadPoolAsio& thPool);

	void startTestDbPool();

	void startEventManagerTest();

	SF::CORE::Ptr<SF::CORE::POOL::ThreadPoolAsioSingleton> m_pThrPool;
	SF::MON::EVENT::EventManager& m_event_manager;
};


} /* namespace SIM */
} /* namespace SF */
#endif /* SIMTESTER_H_ */
