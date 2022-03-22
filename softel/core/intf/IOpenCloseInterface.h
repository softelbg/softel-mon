/*
 * IOpenCloseInterface.h
 *
 *  Created on: 03.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef IOPENCLOSEINTERFACE_H_
#define IOPENCLOSEINTERFACE_H_

namespace SF {
namespace CORE {
namespace INTF {

class IOpenCloseInterface {
public:
	IOpenCloseInterface();
	virtual ~IOpenCloseInterface();

	virtual void open()=0;
	virtual void close()=0;

};

} /* namespace INTF */
} /* namespace CORE */
} /* namespace SF */

#endif /* IOPENCLOSEINTERFACE_H_ */
