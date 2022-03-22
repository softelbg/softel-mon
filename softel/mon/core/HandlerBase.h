/*
 * HandlerBase.h
 *
 *  Created on: Nov 30, 2013
 *      Author: stan
 */

#ifndef HANDLERBASE_H_
#define HANDLERBASE_H_

#include <softel/core/CObject.h>

namespace SF {
namespace MON {
namespace CORE {

class HandlerBase: public SF::CORE::CObject {
public:
	HandlerBase();
	virtual ~HandlerBase();
};

} /* namespace CORE */
} /* namespace MON */
} /* namespace SF */
#endif /* HANDLERBASE_H_ */
