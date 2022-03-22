/*
 * PollerSnmp.h
 *
 *  Created on: Nov 30, 2013
 *      Author: stan
 */

#ifndef POLLERSNMP_H_
#define POLLERSNMP_H_

#include "PollerBase.h"

namespace SF {
namespace MON {

class PollerSnmp: public SF::MON::PollerBase {
public:
	PollerSnmp();
	virtual ~PollerSnmp();
};

} /* namespace MON */
} /* namespace SF */
#endif /* POLLERSNMP_H_ */
