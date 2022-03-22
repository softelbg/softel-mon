/*
 * SimDbPool.h
 *
 *  SIM DbPool Singleton
 *
 *  Created on: Apr 19, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef SIMDBPOOL_H_
#define SIMDBPOOL_H_

#include <softel/core/pool/DbPool.h>
#include <softel/core/commonDefs.h>

namespace SF {
namespace SIM {

class SimDbPool: public SF::CORE::POOL::DbPool {
public:

	virtual ~SimDbPool();

	static void initSimPool(enum DbType t, size_t numconn);
	static void initSimPool(enum DbType t, size_t numconn,
			SF::CORE::Ptr<SF::CORE::POOL::ThreadPoolAsio>& threadPool);
	static SF::CORE::Ptr<SimDbPool> getInstance();

private:
	SimDbPool(enum DbType t, size_t numconn);
	SimDbPool(enum DbType t, size_t numconn,
			SF::CORE::Ptr<SF::CORE::POOL::ThreadPoolAsio>& threadPool);

	static SF::CORE::Ptr<SimDbPool> m_pInstance;
};

} /* namespace SIM */
} /* namespace SF */
#endif /* SIMDBPOOL_H_ */
