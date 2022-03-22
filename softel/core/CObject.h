/*
 * CObject.h
 *
 *	Base Object Implementation
 *
 *  Created on: 12.05.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef COBJECT_H_
#define COBJECT_H_

#include "IObjectInterface.h"

namespace SF {
namespace CORE {

class CObject: public virtual IObjectInterface {
public:
	virtual ~CObject();

	virtual std::string toString() const;
	virtual std::string toXml() const;
	virtual std::string toHash() const;
	virtual std::string toJson() const;

};

} /* namespace CORE */
} /* namespace SF */
#endif /* COBJECT_H_ */
