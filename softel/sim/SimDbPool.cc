/*
 * SimDbPool.cc
 *
 *  Created on: Apr 19, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "SimDbPool.h"
#include <cassert>

using namespace std;
using namespace SF::CORE;
using namespace SF::CORE::POOL;

namespace SF {
namespace SIM {

Ptr<SimDbPool> SimDbPool::m_pInstance;

SimDbPool::SimDbPool(enum DbType t, size_t numconn) :
	DbPool(t, numconn) {
	// TODO Auto-generated constructor stub

}

SimDbPool::SimDbPool(enum DbType t, size_t numconn, Ptr<ThreadPoolAsio>& threadPool) :
	DbPool(t, numconn, threadPool) {
	// TODO Auto-generated constructor stub

}

SimDbPool::~SimDbPool() {
	// TODO Auto-generated destructor stub
}

void SimDbPool::initSimPool(enum DbType t, size_t numconn) {
	assert(!m_pInstance);
	m_pInstance.reset(new SimDbPool(t, numconn));
}

void SimDbPool::initSimPool(enum DbType t, size_t numconn, Ptr<ThreadPoolAsio>& threadPool) {
	assert(!m_pInstance);
	m_pInstance.reset(new SimDbPool(t, numconn, threadPool));
}

/**
 * Should always be after initial init()
 * Not locked as it should not be necessary...
 */
Ptr<SimDbPool> SimDbPool::getInstance() {
	assert(m_pInstance);
	return m_pInstance;
}

} /* namespace SIM */
} /* namespace SF */
