/*
 * CAlertListBase.h
 *
 *	Base Alert List.
 *	Should contain list of base alerts.
 *
 *  Created on: 06.11.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CALERTLISTBASE_H_
#define CALERTLISTBASE_H_

#include <vector>
#include "COssAlert.h"
#include <softel/core/intf/CListInterface.h>
#include <softel/db/CPosIndex.h>

namespace SF {
namespace OSS {
namespace ALERT {

class CAlertListBase: public SF::CORE::INTF::CListInterface {
public:
	CAlertListBase();
	virtual ~CAlertListBase();

	void clear();
	size_t size() const;

	void add(COssAlert* pAlert);
	void add(const std::string& key, COssAlert* pAlert);

	COssAlert* getAlert(size_t pos);
	COssAlert* getAlert(const std::string& key);

	void dumpInfo();

private:
	std::vector<COssAlert*> m_pAlertList;
	SF::DB::CPosIndex m_index;

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CALERTLISTBASE_H_ */
