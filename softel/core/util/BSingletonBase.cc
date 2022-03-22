/*
 * BSingletonBase.cc
 *
 *  Created on: 29.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BSingletonBase.h"

namespace SF {
namespace CORE {
namespace UTIL {

BSingletonBase* BSingletonBase::m_pInstance = 0;

BSingletonBase::BSingletonBase() {
	// TODO Auto-generated constructor stub

}

BSingletonBase::~BSingletonBase() {
	// TODO Auto-generated destructor stub
}

BSingletonBase* BSingletonBase::BSingletonBase::getInstance() {

	if (m_pInstance == NULL) {
		m_pInstance = new BSingletonBase();
	}

	return m_pInstance;
}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */
