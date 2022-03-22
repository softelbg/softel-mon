/*
 * CWorkerBase.h
 *
 *	Base Worker class.
 *	Represents a Worker Thread which should not exit while worker is alive.
 *	Worker Job should be implemented in subclasses - there is the needed job to do.
 *
 *  Created on: 09.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CWORKERBASE_H_
#define CWORKERBASE_H_

#include "CThreads.h"
#include "CWorkerJob.h"

namespace SF {
namespace CORE {
namespace UTIL {

class CWorkerBase : public CThreads {

public:
	CWorkerBase();
	virtual ~CWorkerBase();

	virtual void setFree();
	virtual void setBusy();
	virtual bool isFree();

	virtual bool setJob(CWorkerJob* pJob);

protected:

	int run();
	void doWorkerJob();

private:

	bool			m_free;

	CWorkerJob*		m_pJob;

};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

#endif /* CWORKERBASE_H_ */
