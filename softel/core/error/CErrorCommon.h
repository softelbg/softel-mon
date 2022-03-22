/*
 * CErrorCommon.h
 *
 * Common Error. Should be used for common errors.
 *
 *  Created on: 14.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CERRORCOMMON_H_
#define CERRORCOMMON_H_

#include "BErrorBase.h"


namespace SF {
namespace CORE {
namespace ERROR {

class CErrorCommon: public BErrorBase {
public:
	CErrorCommon(const std::string& message);
	virtual ~CErrorCommon();
};

} /* namespace ERROR */
} /* namespace CORE */
} /* namespace SF */

#endif /* CERRORCOMMON_H_ */
