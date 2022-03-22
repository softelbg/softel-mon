/*
 * Sample.cc
 *
 *  Created on: 10.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "Sample.h"
#include <softel/core/system/CSystemTools.h>

namespace SF {
namespace MON {
namespace CORE {

template<class T>
Sample<T>::Sample(T value) :
	m_value(value){
	m_time.setNow();
}

template<class T>
Sample<T>::Sample(T value, long timestamp) :
	m_value(value), m_time(timestamp) {

}

template<class T>
Sample<T>::~Sample() {
	// TODO Auto-generated destructor stub
}

template<class T>
T Sample<T>::value() {
	return m_value;
}

template<class T>
SF::CORE::UTIL::DateTime Sample<T>::time() {
	return m_time;
}

} /* namespace CORE */
} /* namespace MON */
} /* namespace SF */
