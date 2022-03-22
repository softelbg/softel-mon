/*
 * PollerBase.h
 *
 *  Created on: Nov 30, 2013
 *      Author: stan
 */

#ifndef POLLERBASE_H_
#define POLLERBASE_H_

#include "CObject.h"
#include "IPollerInterface.h"

namespace SF {
namespace MON {

class PollerBase: public SF::CORE::CObject, virtual IPollerInterface {
public:
	PollerBase();
	virtual ~PollerBase();

private:

};

} /* namespace MON */
} /* namespace SF */
#endif /* POLLERBASE_H_ */
