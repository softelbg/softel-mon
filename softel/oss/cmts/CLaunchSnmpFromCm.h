/*
 * CLaunchSnmpFromCm.h
 *
 *  Created on: 25.01.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 *
 */

#ifndef CLaunchSnmpFromCm_H
#define CLaunchSnmpFromCm_H

#include <stdio.h>

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "SnmpOids.h"
#include "CCmtsCm.h"
#include "CSnmpOid.h"
#include "CCmtsToolsSql.h"

#include <softel/core/inc/ErrorMsg.h>
#include <softel/db/CSqlDbFactory.h>
#include <softel/db/CSqlConnPool.h>
#include <softel/core/net/CIp.h>
#include <softel/core/util/CThreads.h>
#include <softel/core/error/CErrorCommon.h>

namespace SF {
namespace OSS {
namespace CMTS {


class CLaunchSnmpFromCm : public SF::CORE::UTIL::CThreads
{

public:
	CLaunchSnmpFromCm(CCmtsCm* cm, std::vector<CSnmpOid>& snmp_oid_cm);

	~CLaunchSnmpFromCm();

protected:

	int run();

private:

	CCmtsCm* m_pCm;

	std::vector<CSnmpOid>& m_snmp_oid_cm;

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif
