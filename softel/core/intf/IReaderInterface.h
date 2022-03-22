/*
 * IReaderInterface.h
 *
 *  Created on: 03.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef IREADERINTERFACE_H_
#define IREADERINTERFACE_H_

namespace SF {
namespace CORE {
namespace INTF {

class IReaderInterface {
public:
	IReaderInterface();
	virtual ~IReaderInterface();

	virtual void read()=0;

};

} /* namespace INTF */
} /* namespace CORE */
} /* namespace SF */

#endif /* IREADERINTERFACE_H_ */
