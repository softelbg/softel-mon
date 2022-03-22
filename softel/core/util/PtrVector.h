/*
 * PtrVector.h
 *
 *	Vector of Ptr(s)
 *
 *  Created on: 18.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef PTRVECTOR_H_
#define PTRVECTOR_H_

#include <vector>
#include <softel/core/Ptr.h>

namespace SF {
namespace CORE {
namespace UTIL {

/*
 *
 */
template<class T>
class PtrVector: public std::vector<SF::CORE::Ptr<T> > {
public:

protected:
	bool validatePos(size_t pos);

};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

#endif /* PTRVECTOR_H_ */
