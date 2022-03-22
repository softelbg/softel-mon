/*
 * WorkItemBase.h
 *
 *	Base Work Item used to post jobs to thread pool.
 *
 *  Created on: 16.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef WORKITEMBASE_H_
#define WORKITEMBASE_H_

#include <softel/core/intf/CRunnableInterface.h>
#include <softel/db/CParamSet.h>

namespace SF {
namespace CORE {
namespace POOL {

/*
 *
 */
class WorkItemBase: public virtual SF::CORE::INTF::CRunnableInterface,
		public SF::DB::CParamSet {
public:
	WorkItemBase();
	virtual ~WorkItemBase();

	virtual void run() = 0;

	int getId() {
		return m_id;
	}
	void setId(int id) {
		m_id = id;
	}

private:
	int m_id;
};

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */
#endif /* WORKITEMBASE_H_ */
