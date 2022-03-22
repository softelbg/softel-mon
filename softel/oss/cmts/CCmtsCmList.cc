/*
 * CCmtsCmList.cc
 *
 *  Created on: 18.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCmtsCmList.h"

using namespace std;

namespace SF {
namespace OSS {
namespace CMTS {

CCmtsCmList::CCmtsCmList() {

}

CCmtsCmList::~CCmtsCmList() {

}

void CCmtsCmList::addCm(CCmtsCm* cm) {

	// Skip invalid CMs
	if (cm == NULL) {
		return;
	}

	int pos = size();

	// Add at least MAC, SID indexing for the CMs
	idx.setIdx(KEY_COL_CM_MAC, cm->getCmMac(), pos);
	idx.setIdx(KEY_COL_CM_SID, cm->getCmSid(), pos);

	m_cm_list.push_back(cm);

}

/*
 * Get CM by Position.
 */
CCmtsCm* CCmtsCmList::getCm(int pos) {

	if (pos < size() && pos >= 0) {
		return m_cm_list[pos];
	} else {
		return NULL;
	}

}

/**
 * Get CM by <key1>, <key2>
 */
CCmtsCm* CCmtsCmList::getCm(const string& key1, const string& key2) {

	int pos = idx.getIdx(key1, key2);

	if (pos >= 0) {
		return m_cm_list[pos];
	} else {
		return NULL;
	}
}

vector<CCmtsCm*>& CCmtsCmList::getCmList() {
	return m_cm_list;
}

void CCmtsCmList::clear() {

	int i;

	for (i = 0; i < size(); i++) {
		delete(getCm(i));
	}

	m_cm_list.clear();
}

int CCmtsCmList::size() {
	return m_cm_list.size();
}

void CCmtsCmList::dumpInfo() {

	int i;

	for(i = 0; i < size(); i++) {
		cout << "CM[" << i << "] = ";
		cout << " mac:" << m_cm_list[i]->getCmMac();
		cout << " ip:" << m_cm_list[i]->getCmIp();
		cout << " cmtsId:" << m_cm_list[i]->getCmtsId();
		cout << endl;
	}
}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */
