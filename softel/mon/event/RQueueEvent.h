/*
 * RQueueEvent.h
 *
 *  Created on: 07.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef RQUEUEEVENT_H_
#define RQUEUEEVENT_H_

#include "EventBase.h"
#include <softel/core/commonDefs.h>
#include <softel/core/pool/RQueue.h>

namespace SF {
namespace MON {
namespace EVENT {

/*
 *
 */
class RQueueEvent: public SF::CORE::POOL::RQueue<SF::CORE::Ptr<EventBase> > {
public:
	RQueueEvent();
	virtual ~RQueueEvent();
};

} /* namespace EVENT */
} /* namespace MON */
} /* namespace SF */
#endif /* RQUEUEEVENT_H_ */
