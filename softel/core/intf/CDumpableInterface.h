/*
 * CDumpableInterface.h
 *
 *  Created on: 12.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CDUMPABLEINTERFACE_H_
#define CDUMPABLEINTERFACE_H_

namespace SF {
namespace CORE {
namespace INTF {

class CDumpableInterface {
public:
	CDumpableInterface();
	virtual ~CDumpableInterface();

	virtual void dumpInfo()=0;

};

} /* namespace INTF */
} /* namespace CORE */
} /* namespace SF */

#endif /* CDUMPABLEINTERFACE_H_ */
