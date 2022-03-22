/*
 * ScheduledItem.h
 *
 *	Scheduled Item used to wrap a single scheduled item and period
 *
 *  Created on: 30.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef ScheduledItem_H_
#define ScheduledItem_H_

#include <softel/core/CObject.h>
#include <softel/core/intf/CRunnableInterface.h>
#include <softel/core/util/DateTime.h>

namespace SF {
namespace CORE {
namespace POOL {

class ScheduledItem: public SF::CORE::CObject,
					 public virtual SF::CORE::INTF::CRunnableInterface {
public:
   ScheduledItem();
   ScheduledItem(std::string id, long p);
   virtual ~ScheduledItem();

   virtual void run() = 0;

   void setPeriod(long p) {
	   m_period = p;
   }

   long getPeriod() const {
	   return m_period;
   }

	std::string getId() const {
		return m_id;
	}

	void setId(std::string id) {
		m_id = id;
	}

private:
   std::string m_id;
   long m_period;
   SF::CORE::UTIL::DateTime m_date;
};

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */

#endif /* ScheduledItem_H_ */
