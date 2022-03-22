/*
 * DateTime.h
 *
 *	Date and Time presentation
 *
 *  Created on: Sep 17, 2013
 *      Author: stan
 */

#ifndef DATETIME_H_
#define DATETIME_H_

#include <softel/core/CObject.h>

namespace SF {
namespace CORE {
namespace UTIL {

class DateTime: public SF::CORE::CObject {
public:
	DateTime();
	DateTime(long timestamp);
	virtual ~DateTime();

	long getTimeStamp() const;
	long getElapsed() const;

	void setTimeStamp(long timestamp);
	void setNow();

private:
	long m_timestamp;
};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */
#endif /* DATETIME_H_ */
