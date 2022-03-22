/*
 * CErrorCommon.cc
 *
 *  Created on: 14.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CErrorCommon.h"



namespace SF {
namespace CORE {
namespace ERROR {

CErrorCommon::CErrorCommon(const std::string& message) :
	BErrorBase(message) {

}

CErrorCommon::~CErrorCommon() {
	// TODO Auto-generated destructor stub
}


} /* namespace ERROR */
} /* namespace CORE */
} /* namespace SF */
