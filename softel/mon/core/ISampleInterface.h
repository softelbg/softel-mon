/*
 * ISampleInterface.h
 *
 *	Sample Interface.
 *
 *  Created on: 10.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef ISAMPLEINTERFACE_H_
#define ISAMPLEINTERFACE_H_

#include <softel/core/util/DateTime.h>

namespace SF {
namespace MON {
namespace CORE {

/*
 *
 */
template <class T>
class ISampleInterface {

public:
	virtual T value()=0;
	virtual SF::CORE::UTIL::DateTime time()=0;

};

} /* namespace CORE */
} /* namespace MON */
} /* namespace SF */
#endif /* ISAMPLEINTERFACE_H_ */
