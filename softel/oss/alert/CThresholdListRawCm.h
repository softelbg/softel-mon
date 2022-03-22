/*
 * CThresholdListRawCm.h
 *
 *  Created on: 03.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTHRESHOLDLISTRAWCM_H_
#define CTHRESHOLDLISTRAWCM_H_

#include "CThresholdListRaw.h"

namespace SF {
namespace OSS {
namespace ALERT {

class CThresholdListRawCm: public CThresholdListRaw {
public:
	CThresholdListRawCm();
	virtual ~CThresholdListRawCm();

	virtual void init();

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTHRESHOLDLISTRAWCM_H_ */
