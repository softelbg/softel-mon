/*
 * CNetworkElementList.h
 *
 *	Network Elemets List - basic list actions needed...
 *
 *	TODO: Still Lists are not very well done without the use of templates...
 *
 *  Created on: 27.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CNETWORKELEMENTLIST_H_
#define CNETWORKELEMENTLIST_H_

#include "BNetworkElementBase.h"

#include <softel/core/intf/CListInterface.h>
#include <softel/core/intf/CDumpableInterface.h>
#include <softel/db/CPosIndex.h>

namespace SF {
namespace OSS {
namespace NMS {

class CNetworkElementList:
			public SF::CORE::INTF::CListInterface,
			public SF::CORE::INTF::CDumpableInterface {
public:
	CNetworkElementList();
	virtual ~CNetworkElementList();

	virtual void clear();
	virtual size_t size() const;

	void add(BNetworkElementBase* pNetElem);
	void add(BNetworkElementBase* pNetElem, const std::string& key);

	BNetworkElementBase* get(size_t pos);
	BNetworkElementBase* get(const std::string& key);

	void dumpInfo();

private:

	std::vector<BNetworkElementBase*> m_net_elem_list;
	SF::DB::CPosIndex m_idx;

};

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CNETWORKELEMENTLIST_H_ */
