/*
 * CObject.cc
 *
 *  Created on: 12.05.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include <sstream>

#include "CObject.h"
#include <softel/core/util/CStringUtil.h>

namespace SF {
namespace CORE {

CObject::~CObject() {
	// TODO Auto-generated destructor stub
}

std::string CObject::toString() const {
	std::stringstream ss;

	ss << __FILE__ << "::this = " << this;

	return ss.str();
}

std::string CObject::toXml() const {

	std::stringstream ss;

	ss << "<this>" << this << "</this>";

	return ss.str();
}

std::string CObject::toHash() const {
	return SF::CORE::UTIL::CStringUtil::toJavaHash(this->toString());
}

std::string CObject::toJson() const {

	std::stringstream ss;

	ss << "{ \"this\": \"" << this << "\" }";

	return ss.str();
}

} /* namespace CORE */
} /* namespace SF */
