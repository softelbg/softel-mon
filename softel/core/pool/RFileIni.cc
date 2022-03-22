/*
 * RFileIni.cc
 *
 *  Created on: 29.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "RFileIni.h"

using namespace std;
using namespace SF::CORE;
using namespace SF::CORE::LOCK;

namespace SF {
namespace CORE {
namespace POOL {

RFileIni::RFileIni(const std::string& fileName) :
		m_ini_file(fileName) {

}

RFileIni::~RFileIni() {
	// TODO Auto-generated destructor stub
}

string RFileIni::get(const string& key) {
	ReadSynchronized lock(m_mutex);
	return m_ini_file.get(key);
}

string RFileIni::get(const string& section, const string& key) {
	ReadSynchronized lock(m_mutex);
	return m_ini_file.get(section, key);
}

void RFileIni::dumpInfo() {
	ReadSynchronized lock(m_mutex);
	m_ini_file.dumpInfo();
}

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */

