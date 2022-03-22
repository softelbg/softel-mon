/*
 * CEventThreshold.h
 *
 *	Base Threshold Event.
 *	Should raise an Event if there is a parameter out of the threshold limits.
 *
 *	For example if there is a threshold for dwn snr < 35dB
 *	and there is a record with snr=30, an event for low snr
 *	should be produced for an cmts element.
 *
 *  Created on: 13.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CEVENTTHRESHOLD_H_
#define CEVENTTHRESHOLD_H_

#include "CEventBase.h"
#include "CThresholdBase.h"

namespace SF {
namespace OSS {
namespace ALERT {

class CEventThreshold: public CEventBase {
public:
	CEventThreshold();
	virtual ~CEventThreshold();

protected:

	CThresholdBase* pThreshold;

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CEVENTTHRESHOLD_H_ */
