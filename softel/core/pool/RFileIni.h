/*
 * RFileIni.h
 *
 *  Base thread-sage ini file Singleton
 *
 *  Created on: 29.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef RFileIni_H_
#define RFileIni_H_

#include <softel/core/CObject.h>
#include <softel/core/Ptr.h>
#include <softel/core/util/BFileIniBase.h>
#include <softel/core/lock/BoostSynchronized.h>

namespace SF {
namespace CORE {
namespace POOL {

class RFileIni: public SF::CORE::CObject {
public:
	RFileIni(const std::string& fileName);
	virtual ~RFileIni();

	std::string get(const std::string& key);
	std::string get(const std::string& section, const std::string& key);

	void dumpInfo();

protected:
	SF::CORE::LOCK::RWMutex m_mutex;
	SF::CORE::UTIL::BFileIniBase m_ini_file;
};

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */

#endif /* RFileIni_H_ */
