/*
 * CIpTools.h
 *
 *  Created on: 10.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CIPTOOLS_H_
#define CIPTOOLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <stdarg.h>

#include <string>

namespace SF {
namespace CORE {
namespace NET {

class CIpTools {

public:
	CIpTools();
	~CIpTools();


	bool snmpResetHost(const std::string& ip);

private:



};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */


#endif /* CIPTOOLS_H_ */
