/*
 * SimFileIni.h
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

#ifndef SimFileIni_H_
#define SimFileIni_H_

#include <softel/core/pool/RFileIni.h>

namespace SF {
namespace SIM {

class SimFileIni: public SF::CORE::POOL::RFileIni {
public:
	virtual ~SimFileIni();

	static void init();
	static SF::CORE::Ptr<SimFileIni> getInstance();

private:
	SimFileIni();

	static SF::CORE::Ptr<SimFileIni> m_pInstance;
};

} /* namespace SIM */
} /* namespace SF */

#endif /* SimFileIni_H_ */
