/*
 * BaseSingleton.h
 *
 *  Created on: Apr 19, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BASESINGLETON_H_
#define BASESINGLETON_H_

#include <softel/core/CObject.h>
#include <softel/core/Ptr.h>
#include <cassert>

namespace SF {
namespace CORE {

template<class T>
class BaseSingleton: public SF::CORE::CObject {
public:
	virtual ~BaseSingleton() {}

	static void init() {
		m_pInstance.reset(new T());
	}

	static SF::CORE::Ptr<T> getInstance() {
		assert(m_pInstance);
		return m_pInstance;
	}

protected:
	BaseSingleton() {}

	static SF::CORE::Ptr<T> m_pInstance;
};

} /* namespace CORE */
} /* namespace SF */
#endif /* BASESINGLETON_H_ */
