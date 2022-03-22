/*
 * BExceptionBase.cc
 *
 *  Created on: 14.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BExceptionBase.h"


namespace SF {
namespace CORE {
namespace ERROR {

BExceptionBase::BExceptionBase(const std::string& message) :
	BErrorBase(message) {
	// TODO Auto-generated constructor stub

}

BExceptionBase::~BExceptionBase() {
	// TODO Auto-generated destructor stub
}

} /* namespace ERROR */
} /* namespace CORE */
} /* namespace SF */

