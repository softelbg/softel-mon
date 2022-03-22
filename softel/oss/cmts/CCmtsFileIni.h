/*
 * CCmtsFileIni.h
 *
 *	CMTS Mon Ini File (/etc/CMTSMon) Singleton
 *
 *  Created on: 29.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCMTSFILEINI_H_
#define CCMTSFILEINI_H_

#include <softel/core/util/BFileIniBase.h>

namespace SF {
namespace OSS {
namespace CMTS {

class CCmtsFileIni: public SF::CORE::UTIL::BFileIniBase {
public:
	virtual ~CCmtsFileIni();

	static CCmtsFileIni* getInstance();

private:
	CCmtsFileIni();

	static CCmtsFileIni* m_pInstance;

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CCMTSFILEINI_H_ */
