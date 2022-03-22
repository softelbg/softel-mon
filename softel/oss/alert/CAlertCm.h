/*
 * CAlertCm.h
 *
 *	CM Alert representation.
 *
 *  Created on: 24.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CALERTCM_H_
#define CALERTCM_H_

#include <softel/oss/cmts/CCmtsCm.h>
#include <softel/db/CDataRow.h>
#include "COssAlert.h"

namespace SF {
namespace OSS {
namespace ALERT {

class CAlertCm : public COssAlert {

public:
	CAlertCm(SF::OSS::CMTS::CCmtsCm* pCm);
	virtual ~CAlertCm();

	void setCm(SF::OSS::CMTS::CCmtsCm* pCm);

	virtual void initAlert();
	virtual void initAlert(SF::DB::CDataRow& row);

	virtual void saveAlert();
	virtual void doNewAlert();

private:

	void initCmParams();

	SF::OSS::CMTS::CCmtsCm* m_pCm;

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CALERTCM_H_ */
