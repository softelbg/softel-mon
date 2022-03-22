/*
 * CNetworkElementList.cc
 *
 *  Created on: 27.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CNetworkElementList.h"

using namespace std;

namespace SF {
namespace OSS {
namespace NMS {

CNetworkElementList::CNetworkElementList() {
	// TODO Auto-generated constructor stub

}

CNetworkElementList::~CNetworkElementList() {
	// TODO Auto-generated destructor stub
}

void CNetworkElementList::clear() {

	for(int i = 0; i < this->size(); i++) {
		delete this->get(i);
	}

	this->m_net_elem_list.clear();
	this->m_idx.clear();
}

size_t CNetworkElementList::size() const {
	return this->m_net_elem_list.size();
}

/**
 * Add new netelem and use some of its properties to be used as index
 */
void CNetworkElementList::add(BNetworkElementBase* pNetElem) {
	if (pNetElem) {
		this->add(pNetElem, pNetElem->getIp());
	}
}

/**
 * Add a new netelem and create index by key (key should e unique)
 */
void CNetworkElementList::add(BNetworkElementBase* pNetElem, const string& key) {

	this->m_idx.setIdx(key, this->size());

	this->m_net_elem_list.push_back(pNetElem);

}

BNetworkElementBase* CNetworkElementList::get(size_t pos) {

	BNetworkElementBase* pNetElem = NULL;

	if (pos >= 0 && pos < this->size()) {
		pNetElem = this->m_net_elem_list[pos];
	}

	return pNetElem;
}

BNetworkElementBase* CNetworkElementList::get(const string& key) {
	return this->get(this->m_idx.getIdx(key));
}

void CNetworkElementList::dumpInfo() {

	std::cout << __FILE__ << " size: " << this->size() << std::endl;

	for(int i = 0; i < this->size(); i++) {

		BNetworkElementBase* pNetElem = this->get(i);

		if (pNetElem) {
			std::cout << "NetElem[" << i << "] ";
			pNetElem->dumpInfo();
		}

	}
}

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */



