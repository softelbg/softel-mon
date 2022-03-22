/*
 * Exception.h
 *
 *  Created on: Feb 13, 2016
 *      Author: stan
 */

#ifndef SOFTEL_CORE_ERROR_EXCEPTION_H_
#define SOFTEL_CORE_ERROR_EXCEPTION_H_

#include <boost/exception/all.hpp>

namespace SF {
namespace CORE {

class Exception : virtual boost::exception, virtual std::exception{
public:
	virtual ~Exception() {}
};

} /* namespace CORE */
} /* namespace SF */

#endif /* SOFTEL_CORE_ERROR_EXCEPTION_H_ */
