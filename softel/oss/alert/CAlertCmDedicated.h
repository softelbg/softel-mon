/*
 * CAlertCmDedicated.h
 *
 *	CM Alert representation.
 *	Now it should be alert from dedicated monitoring CMs.
 *
 *  Created on: 24.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CALERTCMDEDICATED_H_
#define CALERTCMDEDICATED_H_

#include "COssAlert.h"
#include "CAlertCm.h"

#include <softel/db/CDataRow.h>
#include <softel/oss/cmts/CCmtsCm.h>

namespace SF {
namespace OSS {
namespace ALERT {

class CAlertCmDedicated : public CAlertCm {

public:
	CAlertCmDedicated(SF::OSS::CMTS::CCmtsCm* pCm);
	virtual ~CAlertCmDedicated();

	virtual void initAlert();
	virtual void initAlert(SF::DB::CDataRow& row);

	virtual void saveAlert();
	virtual void doNewAlert();

private:


};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CALERTCMDEDICATED_H_ */
