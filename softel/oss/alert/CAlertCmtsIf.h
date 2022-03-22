/*
 * CAlertCmtsIf.h
 *
 *  Created on: 24.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CALERTCMTSIF_H_
#define CALERTCMTSIF_H_

#include <string>

#include "COssAlert.h"

#include <softel/oss/cmts/CCmtsIf.h>
#include <softel/db/CDataRow.h>

namespace SF {
namespace OSS {
namespace ALERT {

class CAlertCmtsIf : public COssAlert {

public:
	CAlertCmtsIf();
	CAlertCmtsIf(const std::string& cmts_id, const std::string& if_id);
	CAlertCmtsIf(const SF::OSS::CMTS::CCmtsIf& cmtsIf);
	~CAlertCmtsIf();

	virtual void initAlert();
	virtual void initAlert(SF::DB::CDataRow& row);

	virtual void saveAlert();
	virtual void doNewAlert();

private:

	void initParams();

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */


#endif /* CALERTCMTSIF_H_ */
