/*
 * CPingCmtsCm.h
 *
 *	Blizoo Cable Modem Pinger. Set response as requested.
 *
 *  Created on: 19.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CPINGCMTSCM_H_
#define CPINGCMTSCM_H_

#include "CPing.h"

namespace SF {
namespace CORE {
namespace NET {

class CPingCmtsCm: public CPing {
public:
	CPingCmtsCm(const std::string& ip);
	virtual ~CPingCmtsCm();

protected:

	virtual void setResponse();

};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

#endif /* CPINGCMTSCM_H_ */
