/*
 * BConnectionBase.h
 *
 *	Base Connection - should be used in various pools subclassed by specific
 *	connection types (sql, http, net...)
 *
 *  Created on: 23.03.2010
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BConnectionBase_H_
#define BConnectionBase_H_

#include <softel/core/lock/CLockableObject.h>
#include <softel/core/lock/BoostSynchronized.h>
#include <softel/core/util/DateTime.h>

namespace SF {
namespace CORE {
namespace POOL {

class BConnectionBase: public SF::CORE::LOCK::CLockableObject {
public:
   BConnectionBase();
   BConnectionBase(int id);
   virtual ~BConnectionBase();

   void setUsed(bool u) {
	   m_is_used = u;
   }
   bool isUsed() const {
	   return m_is_used;
   }
   bool isFree() const {
	   return !m_is_used;
   }
   void release() {
	   m_is_used = false;
   }
   void reserve() {
	   m_is_used = true;
   }

   void setId(int id) {
	   m_id = id;
   }
   int getId() const {
	   return m_id;
   }

   SF::CORE::UTIL::DateTime getDateCreated() const {
	   return m_date_created;
   }

   void setPeriodValid(long p) {
	   m_period_valid = p;
   }
   long getPeriodValid() const {
	   return m_period_valid;
   }
   bool isPeriodValid() const {
	   return (m_date_created.getElapsed() < m_period_valid);
   }
   bool isExpired() const {
	   return !isPeriodValid();
   }

   bool isAlive() const {
	   return m_is_alive;
   }

   virtual void testAlive() = 0;
   virtual void refresh() = 0;

protected:
   long m_period_valid;
   SF::CORE::UTIL::DateTime m_date_created;

   boost::atomic<int> m_id;
   boost::atomic<bool> m_is_used;
   boost::atomic<bool> m_is_alive;

};

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */

#endif /* BConnectionBase_H_ */
