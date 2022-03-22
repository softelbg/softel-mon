/*
 * ISocketServerInterface.h
 *
 *  Created on: Nov 17, 2013
 *      Author: stan
 */

#ifndef ISOCKETSERVERINTERFACE_H_
#define ISOCKETSERVERINTERFACE_H_

#include <string>

namespace SF {
namespace CORE {
namespace NET {

class ISocketServerInterface {

	virtual void init(int port) = 0;

};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* ISOCKETSERVERINTERFACE_H_ */
