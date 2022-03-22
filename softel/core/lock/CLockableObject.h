/*
 * CLockableObject.h
 *
 *	Base Lockable Object - should contain locking mechanism (mutex...).
 *
 *  Created on: 13.05.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CLOCKABLEOBJECT_H_
#define CLOCKABLEOBJECT_H_

#include <softel/core/IObjectInterface.h>
#include <softel/core/lock/BoostSynchronized.h>

namespace SF {
namespace CORE {
namespace LOCK {

class CLockableObject: public SF::CORE::LOCK::RWMutex, public CObject {
public:

};

class Monitor {
public:
    virtual ~Monitor() {};

    void wait();
    void notify_one();
    void notify_all();

private:
    mutable Mutex mx_;
    mutable Condition cv_;
};

} /* namespace LOCK */
} /* namespace CORE */
} /* namespace SF */
#endif /* CLOCKABLEOBJECT_H_ */
