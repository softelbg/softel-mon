/*
 * BExceptionBase.h
 *
 *  Created on: 14.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BEXCEPTIONBASE_H_
#define BEXCEPTIONBASE_H_

#include "BErrorBase.h"



namespace SF {
namespace CORE {
namespace ERROR {

class BExceptionBase: public BErrorBase {
public:
	BExceptionBase(const std::string& message);
	virtual ~BExceptionBase();


};

} /* namespace ERROR */
} /* namespace CORE */
} /* namespace SF */

#endif /* BEXCEPTIONBASE_H_ */
