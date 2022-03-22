/*
 * CThreadPoolBase.h
 *
 * 	Thread Pool.
 * 	Should has pool of workers and a job queue.
 *
 *  Created on: 08.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTHREADPOOLBASE_H_
#define CTHREADPOOLBASE_H_

#include <vector>
#include <iostream>
#include <sstream>
#include <string>

#include "CWorkerBase.h"
#include "CWorkerJob.h"

namespace SF {
namespace CORE {
namespace UTIL {

class CThreadPoolBase {
public:
	CThreadPoolBase();
	CThreadPoolBase(int size);
	virtual ~CThreadPoolBase();

	virtual int size();
	virtual void clear();
	virtual void initialize(int size);

	virtual CWorkerBase* get(); // Get a Free Worker Thread

	virtual void addJob(CWorkerJob* job);
	virtual void processQueue(); // Should be called frequently
	virtual int getJobQueueSize();

protected:

	virtual void add();
	virtual void free(int pos);

	int 					m_count_free;
	int						m_pos_next_free;

	std::vector<CWorkerBase*>	m_pool;

	std::vector<CWorkerJob*>		m_job_queue;

};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

#endif /* CTHREADPOOLBASE_H_ */
