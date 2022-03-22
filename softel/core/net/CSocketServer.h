/*
 * CSocketServer.h
 *
 *  Created on: 03.02.2013
 *      Author: STAN
 *
 *      www.softel.bg	All rights reserved
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSOCKETSERVER_H_
#define CSOCKETSERVER_H_

#include "CSocket.h"

namespace SF {
namespace CORE {
namespace NET {

class CSocketServer: public CSocket {
public:
	CSocketServer();
	virtual ~CSocketServer();

	virtual void initSocket();

protected:
	virtual void setSocketOptions();

};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

#endif /* CSOCKETSERVER_H_ */
