/*
 * CThresholdAlert.h
 *
 *  Created on: 03.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTHRESHOLDALERT_H_
#define CTHRESHOLDALERT_H_

#include <softel/oss/alert/CThresholdListRawCm.h>
#include <softel/oss/alert/CThresholdListRawCmtsIf.h>
#include <softel/oss/alert/CThresholdOidList.h>
#include <softel/oss/alert/CCmtsCmLatestRaw.h>
#include <softel/oss/alert/CCmtsIfRawCmCounter.h>
#include <softel/oss/alert/CCmtsIfCmCounter.h>
#include <softel/oss/alert/CAlertListCmtsIfCmCount.h>

namespace SF {
namespace OSS {
namespace EXEC {

class CThresholdAlert {
public:
	CThresholdAlert();
	virtual ~CThresholdAlert();

	void start();

	void dumpInfo() const;

private:

	SF::OSS::ALERT::CThresholdListRawCm 	m_threshold_list_raw_cm;
	SF::OSS::ALERT::CThresholdListRawCmtsIf m_threshold_list_raw_if;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTHRESHOLDALERT_H_ */
