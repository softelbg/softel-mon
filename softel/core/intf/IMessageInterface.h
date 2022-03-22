/*
 * IMessageInterface.h
 *
 *  Created on: 14.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef IMESSAGEINTERFACE_H_
#define IMESSAGEINTERFACE_H_

#include <string>


namespace SF {
namespace CORE {
namespace INTF {


class IMessageInterface {
public:
	virtual ~IMessageInterface();

	virtual const std::string& getMessage() const = 0;
	virtual void setMessage(const std::string& message)=0;

};

} /* namespace INTF */
} /* namespace CORE */
} /* namespace SF */

#endif /* IMESSAGEINTERFACE_H_ */
