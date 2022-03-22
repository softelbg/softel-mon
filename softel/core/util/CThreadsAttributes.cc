/**
 * Thread Attributes Wrapper
 *
 *	@author sgeorgiev
 *
 *		www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CThreadsAttributes.h"

namespace SF {
namespace CORE {
namespace UTIL {

CThreadsAttributes::CThreadsAttributes(size_t stack_size, bool detached) :
	m_init_success(false) {
	if (::pthread_attr_init(&m_attr) != 0)
		return;

	if (stack_size == 0) {
		//stack_size = PTHREAD_STACK_MIN; // 1MB
		stack_size = 1048576;
	}

	if (stack_size && ::pthread_attr_setstacksize(&m_attr, stack_size) != 0) {
		::pthread_attr_destroy(&m_attr);
		return;
	}

	if (::pthread_attr_setdetachstate(&m_attr, detached ? 1 : 0) != 0) {
		::pthread_attr_destroy(&m_attr);
		return;
	}

	m_init_success = true;
}

CThreadsAttributes::~CThreadsAttributes() {
	if (m_init_success) {
		::pthread_attr_destroy(&m_attr);
	}
}

bool CThreadsAttributes::init_success() const {
	return m_init_success;
}

const pthread_attr_t& CThreadsAttributes::attributes() const {
	return m_attr;
}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */


