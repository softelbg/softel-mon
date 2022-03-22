/*
 * Sample.h
 *
 *	Base Sample implementation.
 *	Still not clear what should contain except the basic time->value data...
 *	Also what type of data...
 *
 *  Created on: 10.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef SAMPLE_H_
#define SAMPLE_H_

#include <softel/core/CObject.h>
#include <softel/core/util/DateTime.h>
#include "ISampleInterface.h"

namespace SF {
namespace MON {
namespace CORE {

template <class T>
class Sample : virtual public ISampleInterface<T>, public SF::CORE::CObject {
public:
	Sample(T value);
	Sample(T value, long timestamp);
	virtual ~Sample();

	T value();
	SF::CORE::UTIL::DateTime time();

private:
	SF::CORE::UTIL::DateTime m_time;
	T m_value;

};

} /* namespace CORE */
} /* namespace MON */
} /* namespace SF */
#endif /* SAMPLE_H_ */
