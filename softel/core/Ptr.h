/*
 * Ptr.h
 *
 * Base smart pointer representation.
 * Currently wraps shared_ptr...
 *
 *  Created on: 14.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef PTR_H_
#define PTR_H_

#include <memory>

namespace SF {
namespace CORE {


/*
 *
 */

template<class T>
class Ptr : public std::shared_ptr<T> {
public:
	using std::shared_ptr<T>::shared_ptr;

};


} /* namespace CORE */
} /* namespace SF */
#endif /* PTR_H_ */
