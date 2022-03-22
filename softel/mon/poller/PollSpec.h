/*
 * PollSpec.h
 *
 *  Created on: Nov 30, 2013
 *      Author: stan
 */

#ifndef POLLSPEC_H_
#define POLLSPEC_H_

#include <softel/mon/core/SpecBase.h>

namespace SF {
namespace MON {

class PollSpec: public SF::MON::CORE::SpecBase {
public:
	PollSpec();
	virtual ~PollSpec();

private:
	ProtocolSpec	m_protocol;

};

} /* namespace MON */
} /* namespace SF */
#endif /* POLLSPEC_H_ */
