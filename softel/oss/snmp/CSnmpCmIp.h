/*
 * CSnmpCmIp.h
 *
 * 	SNMP CM tools. Should retreive CM IP when MAC is available.
 *
 *
 *  Created on: 19.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSNMPCMIP_H_
#define CSNMPCMIP_H_

#include "BSnmpCmtsBase.h"

#include <softel/db/CDataRow.h>

namespace SF {
namespace OSS {
namespace SNMP {

class CSnmpCmIp: public BSnmpCmtsBase {
public:
	CSnmpCmIp();
	CSnmpCmIp(SF::DB::CDataRow& rowCmts);
	virtual ~CSnmpCmIp();

	void init();
	void init(SF::DB::CDataRow& rowCmts);

	std::string getCmIp();

protected:

	bool initCmInfo();

};

} /* namespace SNMP */
} /* namespace OSS */
} /* namespace SF */

#endif /* CSNMPCMIP_H_ */
