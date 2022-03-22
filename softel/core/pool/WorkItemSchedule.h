/*
 * WorkItemSchedule.h
 *
 * Work Item for Scheduled items to start
 *
 *  Created on: 27.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef WorkItemSchedule_H_
#define WorkItemSchedule_H_

#include <softel/core/pool/WorkItemBase.h>
#include <softel/core/pool/ScheduledItem.h>
#include <softel/core/Ptr.h>
#include <vector>

namespace SF {
namespace CORE {
namespace POOL {

/*
 * Base Work Item for Scheduled operations
 */
class WorkItemSchedule: public SF::CORE::POOL::WorkItemBase {
public:
	WorkItemSchedule();
	virtual ~WorkItemSchedule();

	virtual void run();

	void add(const SF::CORE::Ptr<ScheduledItem>& si);

	void stop();

private:
	std::vector<SF::CORE::Ptr<ScheduledItem>> m_scheduled_items;
};

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */
#endif /* WorkItemSchedule_H_ */
