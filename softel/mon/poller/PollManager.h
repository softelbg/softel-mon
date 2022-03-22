/*
 * PollManager.h
 *
 *  Created on: Nov 30, 2013
 *      Author: stan
 */

#ifndef POLLMANAGER_H_
#define POLLMANAGER_H_

#include "ManagerBase.h"

namespace SF {
namespace MON {

class PollManager: public SF::MON::CORE::ManagerBase {
public:
	PollManager();
	virtual ~PollManager();
};

} /* namespace MON */
} /* namespace SF */
#endif /* POLLMANAGER_H_ */
