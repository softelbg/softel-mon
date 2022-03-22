/*
 * CListInterface.h
 *
 *	List Interface. Should make interface for Lists.
 *
 *  Created on: 03.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CLISTINTERFACE_H_
#define CLISTINTERFACE_H_

#include <string>

namespace SF {
namespace CORE {
namespace INTF {

class CListInterface {
public:
	CListInterface();
	virtual ~CListInterface();

	virtual void clear()=0;
	virtual size_t size() const = 0;

};

} /* namespace INTF */
} /* namespace CORE */
} /* namespace SF */

#endif /* CLISTINTERFACE_H_ */
