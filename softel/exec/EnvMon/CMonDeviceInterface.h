/*
 * CMonDeviceInterface.h
 *
 *  Created on: 12.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CMONDEVICEINTERFACE_H_
#define CMONDEVICEINTERFACE_H_

#include <string>

namespace SF {
namespace OSS {
namespace EXEC {

class CMonDeviceInterface {
public:
	CMonDeviceInterface();
	virtual ~CMonDeviceInterface();

	virtual void sendRequest()=0;

	virtual std::string getResponse()=0;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CMONDEVICEINTERFACE_H_ */
