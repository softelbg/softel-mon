/*
 * CNetworkElementIFList.h
 *
 * 	List of Network Element Interfaces
 *
 *  Created on: 21.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CNETWORKELEMETIFLIST_H_
#define CNETWORKELEMETIFLIST_H_

#include <vector>
#include <string>

#include <softel/core/intf/CListInterface.h>
#include <softel/db/CPosIndex.h>
#include "CNetworkElementIF.h"

namespace SF {
namespace OSS {
namespace NMS {

class CNetworkElementIFList: public SF::CORE::INTF::CListInterface {
public:
	CNetworkElementIFList();
	virtual ~CNetworkElementIFList();

	virtual void clear();
	virtual size_t size() const;

	void addIf(CNetworkElementIF* net_if);

	CNetworkElementIF* getIf(const std::string& key) const;
	CNetworkElementIF* getIf(const std::string& key1, const std::string& key2) const;
	CNetworkElementIF* getIf(size_t pos) const;

	void dumpInfo() const;

private:

	std::vector<CNetworkElementIF*> m_if_list;
	SF::DB::CPosIndex m_idx;

};

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CNETWORKELEMETIFLIST_H_ */
