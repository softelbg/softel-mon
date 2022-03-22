/*
 * CCmtsCmAlertList.h
 *
 *	Cmts CM List which should be monitored and make alerts...
 *
 *  Created on: 19.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCMTSCMALERTLIST_H_
#define CCMTSCMALERTLIST_H_

#include <softel/oss/cmts/CCmtsCmList.h>
#include <softel/oss/cmts/CSnmpOid.h>

namespace SF {
namespace OSS {
namespace EXEC {

class CCmtsCmAlertList {

public:
	CCmtsCmAlertList();
	~CCmtsCmAlertList();

	void initCmList();

	void monitorCmList();

private:

	void doAsynchMonitorCmList(	SF::OSS::CMTS::CCmtsCmList& listCm,
								SF::OSS::CMTS::CCmtsCmList& listGoodCm,
								SF::OSS::CMTS::CCmtsCmList& listAlertCm,
								SF::OSS::CMTS::CCmtsCmList& listNonReachable,
								vector<SF::OSS::CMTS::CSnmpOid>& cmOidList);

	SF::OSS::CMTS::CCmtsCmList m_cm_list;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CCMTSCMALERTLIST_H_ */
