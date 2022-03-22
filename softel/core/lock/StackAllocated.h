/*
 * StackAllocated.h
 *
 *  Created on: 08.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef STACKALLOCATED_H_
#define STACKALLOCATED_H_

#include <softel/core/CObject.h>

namespace SF {
namespace CORE {
namespace LOCK {

class StackAllocated: public SF::CORE::CObject {
protected:
   StackAllocated() {}

private:
   // Disable heap allocation. These are not implemented/linkable.
   //void *operator new(std::size_t s);
   //void *operator new[](std::size_t s);

   // Disable copy. These are not implemented/linkable.
   StackAllocated(const StackAllocated&);
   const StackAllocated& operator=(const StackAllocated&);

};

} /* namespace LOCK */
} /* namespace CORE */
} /* namespace SF */
#endif /* STACKALLOCATED_H_ */
