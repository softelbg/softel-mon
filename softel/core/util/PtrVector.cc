/*
 * PtrVector.cc
 *
 *  Created on: 18.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "PtrVector.h"

namespace SF {
namespace CORE {
namespace UTIL {

template<class T>
bool SF::CORE::UTIL::PtrVector<T>::validatePos(size_t pos) {
	return (pos >= 0 && pos < this->size());
}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */
