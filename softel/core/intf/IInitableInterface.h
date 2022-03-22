/*
 * IInitableInterface.h
 *
 *  Created on: 03.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef IINITABLEINTERFACE_H_
#define IINITABLEINTERFACE_H_

namespace SF {
namespace CORE {
namespace INTF {

class IInitableInterface {
public:
	IInitableInterface();
	virtual ~IInitableInterface();

	virtual void init()=0;

};

} /* namespace INTF */
} /* namespace CORE */
} /* namespace SF */

#endif /* IINITABLEINTERFACE_H_ */
