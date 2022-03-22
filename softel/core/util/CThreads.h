/**
 * Thread Wrapper
 *
 *	@author sgeorgiev
 *
 *		www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef     CThreads_H
#define     CThreads_H

#include <stdio.h>
#include <cstddef>
#include <pthread.h>

#include "CThreadsAttributes.h"

namespace SF {
namespace CORE {
namespace UTIL {

class CThreads {
private:
	// Thread identifier.
	::pthread_t m_id;

	// Flag to indicate thread should stop.  It is up to child classes to
	// respect a request to stop.
	volatile bool m_stop;

	// Used as the startup function for new threads.
	// Calls the thread object's run method.
	static void * starter(void *);

	// Intentionally not implemented.
	CThreads(const CThreads &);
	CThreads & operator=(const CThreads &);

protected:

	// Entry point for concrete subclasses.  Should periodically call
	// shouldStop(), and return when shouldStop() is true.
	virtual int run() = 0;

public:
	CThreads();
	virtual ~CThreads();

	bool operator==(const CThreads & t) const {
		return ::pthread_equal(m_id, t.m_id) != 0;
	}

	// Warning: It is up to subclasses of CThreads to respect a request to
	// stop.
	void stop() {
		m_stop = true;
	}

	bool shouldStop() const {
		return m_stop;
	}

	// You need not replace this start() method; but you will need to
	// call start() after constructing a thread.  Otherwise the thread
	// won't actually be created and started running.
	// Note: stackSize == 0 means to use the system default.
	bool start(std::size_t stackSize = 0, bool bDetachedThread = false);

	bool join();

	::pthread_t getThrId();


};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

#endif      // CThreads_H
