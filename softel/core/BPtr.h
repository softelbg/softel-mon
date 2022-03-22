/*
 * BPtr.h
 *
 *	Base smart pointer representation.
 * 	Currently wraps Boost shared_ptr...
 *
 *  Created on: 14.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BPTR_H_
#define BPTR_H_

#include <boost/shared_ptr.hpp>

namespace SF {
namespace CORE {

/*
 *
 */
template<class T>
class BPtr : public boost::shared_ptr<T> {

};

} /* namespace CORE */
} /* namespace SF */
#endif /* BPTR_H_ */
