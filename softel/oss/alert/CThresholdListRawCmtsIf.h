/*
 * CThresholdListRawCmtsIf.h
 *
 *  Created on: 03.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTHRESHOLDLISTRAWCMTSIF_H_
#define CTHRESHOLDLISTRAWCMTSIF_H_

#include "CThresholdListRaw.h"

namespace SF {
namespace OSS {
namespace ALERT {

class CThresholdListRawCmtsIf: public CThresholdListRaw {
public:
	CThresholdListRawCmtsIf();
	virtual ~CThresholdListRawCmtsIf();

	virtual void init();

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTHRESHOLDLISTRAWCMTSIF_H_ */
