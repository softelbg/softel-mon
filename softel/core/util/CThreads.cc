/**
 * Thread Wrapper
 *
 *	@author sgeorgiev
 *
 *
 *		www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CThreads.h"

namespace SF {
namespace CORE {
namespace UTIL {

void * CThreads::starter(void * p) {

	CThreads * th = static_cast<CThreads *>(p);

	// Call the implementation's thread function.
	int ret = th->run();

	return reinterpret_cast<void *>(ret);
}

CThreads::CThreads() :
		m_id(0), m_stop(false) {
}

CThreads::~CThreads() {
}

bool CThreads::start(std::size_t stackSize, bool bDetachedThread) {
	// Set up attributes.
	CThreadsAttributes attr(stackSize, bDetachedThread);
	if (!attr.init_success())
		return false;

	// Start the thread.
	if (::pthread_create(&m_id, &attr.attributes(), &CThreads::starter, this)
			!= 0)
		return false;

	return true;
}

bool CThreads::join() {
	int status = ::pthread_join(m_id, 0);

	if (status != 0) {
		printf("pthread_join failed. status = %d\n", status);
		return false;
	}

	return true;
}

::pthread_t CThreads::getThrId() {
		return m_id;
}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */


