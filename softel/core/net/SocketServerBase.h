/*
 * SocketServerBase.h
 *
 *  Created on: Nov 17, 2013
 *      Author: stan
 */

#ifndef SOCKETSERVERBASE_H_
#define SOCKETSERVERBASE_H_

#include <softel/core/CObject.h>
#include "ISocketServerInterface.h"

namespace SF {
namespace CORE {
namespace NET {

class SocketServerBase: public SF::CORE::CObject, virtual ISocketServerInterface {
public:
	SocketServerBase();
	virtual ~SocketServerBase();

	void init(int port);

};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* SOCKETSERVERBASE_H_ */
