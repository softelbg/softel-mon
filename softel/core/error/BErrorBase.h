/*
 * BErrorBase.h
 *
 *  Created on: 14.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BERRORBASE_H_
#define BERRORBASE_H_

#include <softel/core/intf/IMessageInterface.h>
#include <softel/core/CObject.h>


namespace SF {
namespace CORE {
namespace ERROR {


class BErrorBase: public SF::CORE::CObject {
public:
	BErrorBase(const std::string& message);
	virtual ~BErrorBase();

	const std::string& getMessage() const;
	void setMessage(const std::string& message);

protected:
	std::string	m_error_message;

};

} /* namespace ERROR */
} /* namespace CORE */
} /* namespace SF */

#endif /* BERRORBASE_H_ */
