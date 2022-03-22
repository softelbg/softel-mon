/*
 * CEventCmtsIf.h
 *
 *	Cmts IFs Threshold Event.
 *	Should check are there any threshold events for the cmts interfaces.
 *
 *  Created on: 13.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CEVENTCMTSIF_H_
#define CEVENTCMTSIF_H_

#include "CEventThreshold.h"

namespace SF {
namespace OSS {
namespace ALERT {

class CEventCmtsIf: public CEventThreshold {
public:
	CEventCmtsIf();
	virtual ~CEventCmtsIf();
};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CEVENTCMTSIF_H_ */
