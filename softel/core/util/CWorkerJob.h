/*
 * CWorkerJob.h
 *
 *	Base Worker Job.
 *	Subclasses should implement their own run where the actual work is done.
 *	Used to make the job in threaded workers.
 *
 *  Created on: 10.09.2012
 *      Author: STAN
 *
 *       www.softel.bg
 *
 *       Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CWORKERJOB_H_
#define CWORKERJOB_H_

namespace SF {
namespace CORE {
namespace UTIL {

class CWorkerJob {
public:
	CWorkerJob();
	virtual ~CWorkerJob();

	bool isCompleted();
	void setCompleted();

	bool isDeletable();

	virtual void run()=0;

	void setShouldDelete(bool shouldDelete);

private:

	bool	m_completed;
	bool	m_should_delete;

};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

#endif /* CWORKERJOB_H_ */
