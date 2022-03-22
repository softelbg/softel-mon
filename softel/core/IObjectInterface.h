/*
 * IObjectInterface.h
 *
 * 	Object Interface - should be base for all classes.
 *
 *  Created on: 12.05.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef IOBJECTINTERFACE_H_
#define IOBJECTINTERFACE_H_

#include <string>

namespace SF {
namespace CORE {

class IObjectInterface {
public:
	virtual ~IObjectInterface() {}

	virtual std::string toString() const = 0;
	virtual std::string toXml() const = 0;
	virtual std::string toHash() const = 0;
	virtual std::string toJson() const = 0;
};

} /* namespace CORE */
} /* namespace SF */
#endif /* IOBJECTINTERFACE_H_ */
