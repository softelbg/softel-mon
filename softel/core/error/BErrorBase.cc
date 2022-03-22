/*
 * BErrorBase.cc
 *
 *  Created on: 14.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BErrorBase.h"


namespace SF {
namespace CORE {
namespace ERROR {

BErrorBase::BErrorBase(const std::string& message) :
	m_error_message(message) {

}

BErrorBase::~BErrorBase() {
	// TODO Auto-generated destructor stub
}

const std::string& BErrorBase::getMessage() const {
	return this->m_error_message;
}

void BErrorBase::setMessage(const std::string& message) {
	this->m_error_message = message;
}

} /* namespace ERROR */
} /* namespace CORE */
} /* namespace SF */
