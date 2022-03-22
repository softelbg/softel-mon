/**
 * Thread Attributes Wrapper
 *
 *	@author sgeorgiev
 *
 *
 *		www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef     CThreadsAttributes_H
#define     CThreadsAttributes_H

#include <stdio.h>
#include <cstddef>
#include <pthread.h>

namespace SF {
namespace CORE {
namespace UTIL {

class CThreadsAttributes {

private:
	pthread_attr_t m_attr;
	bool m_init_success;

public:

	CThreadsAttributes(size_t stack_size, bool detached);
	~CThreadsAttributes();

	bool init_success() const;

	const ::pthread_attr_t& attributes() const;

};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

#endif      // CThreadsAttributes_H
