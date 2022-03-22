/*
 * CAlertCmtsIfCmCount.h
 *
 *  Created on: 06.11.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CALERTCMTSIFCMCOUNT_H_
#define CALERTCMTSIFCMCOUNT_H_

#include "CCmtsIfCmCounter.h"
#include "CAlertCmtsIf.h"
#include "CSqlAlertTools.h"

namespace SF {
namespace OSS {
namespace ALERT {

class CAlertCmtsIfCmCount: public CAlertCmtsIf {
public:
	CAlertCmtsIfCmCount(const std::string& cmts_id, const std::string& if_id);
	CAlertCmtsIfCmCount(const SF::OSS::CMTS::CCmtsIf& cmtsIf);
	virtual ~CAlertCmtsIfCmCount();

	void init(const CCmtsIfCmCounter& ifCmCounter);

	virtual void initAlert();
	virtual void initAlert(SF::DB::CDataRow& row);

	virtual void saveAlert();
	virtual void doNewAlert();

private:

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CALERTCMTSIFCMCOUNT_H_ */
