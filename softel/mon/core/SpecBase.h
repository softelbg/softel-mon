/*
 * SpecBase.h
 *
 *  Created on: Nov 30, 2013
 *      Author: stan
 */

#ifndef SPECBASE_H_
#define SPECBASE_H_

#include <softel/core/CObject.h>

namespace SF {
namespace MON {
namespace CORE {

class SpecBase: public SF::CORE::CObject {
public:
	SpecBase();
	virtual ~SpecBase();
};

} /* namespace CORE */
} /* namespace MON */
} /* namespace SF */
#endif /* SPECBASE_H_ */
