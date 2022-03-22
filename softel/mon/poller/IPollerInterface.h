/*
 * IPollerInterface.h
 *
 *  Created on: Nov 30, 2013
 *      Author: stan
 */

#ifndef IPOLLERINTERFACE_H_
#define IPOLLERINTERFACE_H_

#include <softel/core/Ptr.h>

namespace SF {
namespace MON {

class IPollerInterface {

	void addPollSpec(SF::CORE::Ptr<PollSpec> pollSpec) = 0;
	void removePollSpec(SF::CORE::Ptr<PollSpec> pollSpec) = 0;

};

} /* namespace MON */
} /* namespace SF */
#endif /* IPOLLERINTERFACE_H_ */
