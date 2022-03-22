/*
 * CThreshold.h
 *
 *  Created on: 02.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTHRESHOLD_H_
#define CTHRESHOLD_H_

#include "CThresholdBase.h"
#include <softel/db/CDataRow.h>

namespace SF {
namespace OSS {
namespace ALERT {

class CThreshold: public CThresholdBase {
public:
	CThreshold();
	CThreshold(const SF::DB::CDataRow& dr);
	virtual ~CThreshold();

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTHRESHOLD_H_ */
