/*
 * BSnmpCmtsBase.h
 *
 * Base SNMP CMTS query.
 * Should initialize the CMTS params needed (cmts communities, ip and so on...)
 *
 *  Created on: 14.01.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BSNMPCMTSBASE_H_
#define BSNMPCMTSBASE_H_

#include "BSnmpParamBase.h"
#include <softel/db/CDataRow.h>

namespace SF {
namespace OSS {
namespace SNMP {

class BSnmpCmtsBase: public BSnmpParamBase {
public:
	BSnmpCmtsBase();
	BSnmpCmtsBase(SF::DB::CDataRow& rowCmts);
	virtual ~BSnmpCmtsBase();

	void init();
	void init(SF::DB::CDataRow& rowCmts);

protected:

	bool initCmtsInfo();

};

} /* namespace SNMP */
} /* namespace OSS */
} /* namespace SF */

#endif /* BSNMPCMTSBASE_H_ */
