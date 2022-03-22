/*
 * ProtocolSpec.h
 *
 *  Created on: Nov 30, 2013
 *      Author: stan
 */

#ifndef PROTOCOLSPEC_H_
#define PROTOCOLSPEC_H_

#include <softel/mon/core/SpecBase.h>

namespace SF {
namespace MON {

class ProtocolSpec: public SF::MON::CORE::SpecBase {
public:
	ProtocolSpec();
	virtual ~ProtocolSpec();
};

} /* namespace MON */
} /* namespace SF */
#endif /* PROTOCOLSPEC_H_ */
