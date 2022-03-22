/*
 * CAlertListCmtsIfCmCount.h
 *
 *  Created on: 06.11.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CALERTLISTCMTSIFCMCOUNT_H_
#define CALERTLISTCMTSIFCMCOUNT_H_

#include <vector>

#include "CAlertListBase.h"
#include "CCmtsIfCmCounter.h"
#include "CAlertCmtsIfCmCount.h"
#include <softel/oss/cmts/CCmtsIf.h>

namespace SF {
namespace OSS {
namespace ALERT {

class CAlertListCmtsIfCmCount: public CAlertListBase {
public:
	CAlertListCmtsIfCmCount();
	CAlertListCmtsIfCmCount(const CCmtsIfCmCounter& counter);
	virtual ~CAlertListCmtsIfCmCount();

	void init(const CCmtsIfCmCounter& counter);

	void save();

private:

	void addAlert(	const std::string& cmts_id, const std::string& if_id,
					const std::map<std::string, int>& listCounters);

	void closeAlerts();

	std::vector<SF::OSS::CMTS::CCmtsIf>	m_list_alert_if;
	std::vector<SF::OSS::CMTS::CCmtsIf>	m_list_good_if;

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CALERTLISTCMTSIFCMCOUNT_H_ */
