/*
 * CNetworkElementIFList.cc
 *
 *  Created on: 21.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CNetworkElementIFList.h"

using namespace std;

namespace SF {
namespace OSS {
namespace NMS {

CNetworkElementIFList::CNetworkElementIFList() {
	// TODO Auto-generated constructor stub

}

CNetworkElementIFList::~CNetworkElementIFList() {
	// TODO Auto-generated destructor stub
}

void CNetworkElementIFList::clear() {

	for(int i = 0; i < this->size(); i++) {
		delete this->m_if_list[i];
	}

	this->m_if_list.clear();
	this->m_idx.clear();
}

size_t CNetworkElementIFList::size() const {
	return this->m_if_list.size();
}

void CNetworkElementIFList::addIf(CNetworkElementIF* net_if) {

	vector<string> listKeys = net_if->getIdxKeyList();

	this->m_idx.setIdx(listKeys, this->size());

	for(int i = 0; i < listKeys.size(); i++) {
		this->m_idx.setIdx(listKeys[i], this->size());
	}

	this->m_if_list.push_back(net_if);

}

CNetworkElementIF* CNetworkElementIFList::getIf(const string& key) const {

	int pos = this->m_idx.getIdx(key);

	return this->getIf(pos);
}

CNetworkElementIF* CNetworkElementIFList::getIf(const string& key1,
		const string& key2) const {

	int pos = this->m_idx.getIdx(key1, key2);

	return this->getIf(pos);
}

CNetworkElementIF* CNetworkElementIFList::getIf(size_t pos) const {

	CNetworkElementIF* pNetIf = NULL;

	if (pos >= 0 && pos < this->size()) {
		pNetIf = this->m_if_list[pos];
	}

	return pNetIf;
}

void CNetworkElementIFList::dumpInfo() const {

	for(int i = 0; i < this->size(); i++) {

		CNetworkElementIF* pNetIf = this->getIf(i);

		if (pNetIf) {
			cout << "NetElemIF[" << i << "] = ";
			pNetIf->dumpInfo();
		}

	}
}

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */



