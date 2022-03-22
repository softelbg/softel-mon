/*
 * SimMain.h
 *
 *	Sim Main, used now just for testing...
 *
 *  Created on: 27.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef SIMMAIN_H_
#define SIMMAIN_H_

#include <softel/core/commonDefs.h>
#include <softel/mon/event/EventManager.h>
#include <softel/core/pool/ThreadPoolAsioSingleton.h>

namespace SF {
namespace SIM {

/*
 *
 */
class SimMain {
public:
	SimMain();
	virtual ~SimMain();

	void start();

private:
	void startThreadPool();
	void startDbPool();
	void startWebSrv();
	void startScheduled();

	SF::CORE::Ptr<SF::CORE::POOL::ThreadPoolAsioSingleton> m_pThrPool;
	SF::MON::EVENT::EventManager m_event_manager;
};

} /* namespace SIM */
} /* namespace SF */
#endif /* SIMMAIN_H_ */
