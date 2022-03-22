/*
 * CEventThresholdCmtsCm.h
 *
 *  Created on: 03.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CEVENTTHRESHOLDCMTSCM_H_
#define CEVENTTHRESHOLDCMTSCM_H_

#include "CEventThreshold.h"

namespace SF {
namespace OSS {
namespace ALERT {

class CEventThresholdCmtsCm: public CEventThreshold {
public:
	CEventThresholdCmtsCm();
	virtual ~CEventThresholdCmtsCm();
};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CEVENTTHRESHOLDCMTSCM_H_ */
