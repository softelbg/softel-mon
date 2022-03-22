/*
 * CNetElemUps.h
 *
 *	NetElem UPS
 *
 *  Created on: 25.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CNETELEMUPS_H_
#define CNETELEMUPS_H_

#include "BNetworkElementBase.h"
#include <softel/db/CDataRow.h>

namespace SF {
namespace OSS {
namespace NMS {

class CNetElemUps: public BNetworkElementBase {
public:
	CNetElemUps(const std::string& ip);
	CNetElemUps(const std::string& ip, const std::string& mac);
	CNetElemUps(const SF::DB::CDataRow& row);
	virtual ~CNetElemUps();

	virtual void init();

};

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CNETELEMUPS_H_ */
