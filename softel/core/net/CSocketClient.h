/*
 * CSocketClient.h
 *
 *  Created on: 03.02.2013
 *      Author: STAN
 *
 *      www.softel.bg	All rights reserved
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSOCKETCLIENT_H_
#define CSOCKETCLIENT_H_

#include "CSocket.h"

namespace SF {
namespace CORE {
namespace NET {

class CSocketClient: public CSocket {
public:
	CSocketClient();
	virtual ~CSocketClient();

	virtual void initSocket();

protected:
	virtual void setSocketOptions();

};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

#endif /* CSOCKETCLIENT_H_ */
