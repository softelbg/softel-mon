/*
 * CRunnableInterface.h
 *
 *  Created on: 12.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CRUNNABLEINTERFACE_H_
#define CRUNNABLEINTERFACE_H_

namespace SF {
namespace CORE {
namespace INTF {

class CRunnableInterface {
public:
	CRunnableInterface();
	virtual ~CRunnableInterface();

	virtual void run()=0;

};

} /* namespace INTF */
} /* namespace CORE */
} /* namespace SF */

#endif /* CRUNNABLEINTERFACE_H_ */
