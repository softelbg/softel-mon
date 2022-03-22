/*
 * ManagerBase.h
 *
 *	Base Manager...
 *	Should be provided some RPC mechanism for public interface.
 *	However for now there will be a simpler socket communication in parallel with the api.
 *	In common if there is a method Goo Foo(Bar b) there will be also a jason/xml/...
 *	communication with cmd=Foo, and serialized Bar b data and the result serialized Goo...
 *
 *  Created on: 14.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef MANAGERBASE_H_
#define MANAGERBASE_H_

#include <softel/core/lock/CLockableObject.h>

namespace SF {
namespace CORE {

/*
 *
 */
class ManagerBase: public SF::CORE::LOCK::CLockableObject {
public:
	virtual ~ManagerBase();
};

} /* namespace CORE */
} /* namespace SF */
#endif /* MANAGERBASE_H_ */
