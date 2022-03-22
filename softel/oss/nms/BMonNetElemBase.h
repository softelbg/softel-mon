/*
 * BMonNetElemBase.h
 *
 *	Base Network Element Monitoring
 *
 *  Created on: 25.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BMONNETELEMBASE_H_
#define BMONNETELEMBASE_H_

#include <vector>

#include "BNetworkElementBase.h"
#include "CNetworkElementList.h"
#include "CSqlNmsTools.h"

#include <softel/core/intf/CRunnableInterface.h>

namespace SF {
namespace OSS {
namespace NMS {

class BMonNetElemBase : public SF::CORE::INTF::CRunnableInterface {
public:
	BMonNetElemBase();
	virtual ~BMonNetElemBase();

	virtual void run() = 0;

	virtual void init() = 0;

protected:

	CSqlNmsTools m_sql_tools;

	CNetworkElementList m_net_elem_list;

};

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */

#endif /* BMONNETELEMBASE_H_ */
