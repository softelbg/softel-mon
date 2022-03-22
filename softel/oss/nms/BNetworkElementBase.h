/*
 * BNetworkElementBase.h
 *
 *	Base Network Element representation. Should have basic network element parameters.
 *	Could be a snmp managed network element - should contain at least one ip, snmp communities...
 *
 *  Created on: 21.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BNETWORKELEMENTBASE_H_
#define BNETWORKELEMENTBASE_H_

#include "CNetworkElementIF.h"
#include "CNetworkElementIFList.h"
#include <softel/db/CDataRow.h>
#include <softel/oss/cmts/CSnmpValueSet.h>

namespace SF {
namespace OSS {
namespace NMS {

class BNetworkElementBase {
public:
	BNetworkElementBase();
	BNetworkElementBase(const SF::DB::CDataRow& row);
	virtual ~BNetworkElementBase();

	virtual void init()=0;

	virtual std::string getIp() const;

	std::string getSnmpRComm() const;
	std::string getSnmpVer() const;

	void addIf(CNetworkElementIF* net_if);

	CNetworkElementIFList& getIfList();
	SF::OSS::CMTS::CSnmpValueSet& getSnmpValueSet();

	const std::vector<std::string> getIdxKeyList() const;

	virtual bool isActive() const;

	void dumpInfo();

private:

	SF::DB::CDataRow m_row_params;

	CNetworkElementIFList m_if_list;

	SF::OSS::CMTS::CSnmpValueSet m_value_set;

};

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */

#endif /* BNETWORKELEMENTBASE_H_ */
