/*
 * CLockableObject.cc
 *
 *  Created on: 13.05.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CLockableObject.h"

namespace SF {
namespace CORE {
namespace LOCK {

void Monitor::wait() {
    Synchronized l(mx_);
    cv_.wait(l);
}

void Monitor::notify_one() {
    cv_.notify_one();
}

void Monitor::notify_all() {
    cv_.notify_all();
}


} /* namespace LOCK */
} /* namespace CORE */
} /* namespace SF */
