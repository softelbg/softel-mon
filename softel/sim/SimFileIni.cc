/*
 * SimFileIni.cc
 *
 *  Created on: 29.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "SimFileIni.h"
#include <cassert>

using namespace std;
using namespace SF::CORE;
using namespace SF::CORE::LOCK;

namespace SF {
namespace SIM {

#define SIM_INI_FILE	"/etc/simd.ini"

Ptr<SimFileIni> SimFileIni::m_pInstance;

SimFileIni::SimFileIni() :
	RFileIni(SIM_INI_FILE) {

}

SimFileIni::~SimFileIni() {
	// TODO Auto-generated destructor stub
}

/**
 *
 */
void SimFileIni::init() {
	//WriteSynchronized lock(SimFileIni::m_mutex);
	m_pInstance.reset(new SimFileIni());
}

/**
 * Should always be after initial init()
 * Not locked as it should not be necessary...
 */
Ptr<SimFileIni> SimFileIni::getInstance() {
	assert(m_pInstance);
	return m_pInstance;
}

} /* namespace SIM */
} /* namespace SF */

