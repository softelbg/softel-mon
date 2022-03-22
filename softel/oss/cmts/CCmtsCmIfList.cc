/*
 * CCmtsCmIfList.cc
 *
 *  Created on: 27.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCmtsCmIfList.h"

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::NET;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace OSS {
namespace CMTS {

CCmtsCmIfList::CCmtsCmIfList() {

}

CCmtsCmIfList::CCmtsCmIfList(const std::string& cmts_id) {
	this->setCmtsId(cmts_id);
}

CCmtsCmIfList::~CCmtsCmIfList() {
	this->clear();
}

void CCmtsCmIfList::clear() {
	this->m_list_cm.clear();
	this->m_list_if.clear();
}

size_t CCmtsCmIfList::size() const {
	return this->m_list_cm.size();
}

void CCmtsCmIfList::setCmtsId(const std::string& cmts_id) {
	this->m_param_set.setParam(KEY_COL_CMTS_ID, cmts_id);
}

std::string CCmtsCmIfList::getCmtsId() {
	return this->m_param_set.getParam(KEY_COL_CMTS_ID);
}

/**
 * Add new CM<->IF pair.
 * Should have multiple CM->IFs and IF->CMs, But Unique CM<->IF pairs.
 * TODO: Consider make the unique pair container implementation.
 */
void CCmtsCmIfList::add(const std::string& cm_mac, const std::string& if_id) {

	// Check if the pair is already there
	if (this->m_idx_cmifpair.getIdx(cm_mac, if_id) < 0) {

		this->m_idx_cmifpair.setIdx(cm_mac, if_id, 0);

		this->m_list_cm.insert(pair<std::string, std::string>(cm_mac, if_id));
		this->m_list_if.insert(pair<std::string, std::string>(if_id, cm_mac));
	}

}

void CCmtsCmIfList::add(const CCmtsCm& cm, const std::string& if_id) {
	this->add(cm.getCmMac(), if_id);
}

void CCmtsCmIfList::getList(const std::string& key,
		const multimap<std::string, std::string>& source, vector<std::string>& dest_list) {

	pair<multimap<std::string, std::string>::const_iterator, multimap<std::string, std::string>::const_iterator> pairList;
	pairList = source.equal_range(key);

	dest_list.clear();

	multimap<std::string, std::string>::const_iterator it;
	for (it = pairList.first; it != pairList.second; it++) {
		dest_list.push_back(it->second);
	}

}

void CCmtsCmIfList::getCmList(const std::string& if_id, vector<std::string>& cm_id_list) {
	this->getList(if_id, this->m_list_if, cm_id_list);
}

int CCmtsCmIfList::sizeCm(const std::string& if_id) {
	return this->sizeList(if_id, this->m_list_if);
}

int CCmtsCmIfList::sizeIf(const std::string& cm_mac) {
	return this->sizeList(cm_mac, this->m_list_cm);
}

int CCmtsCmIfList::sizeList(const std::string& key,
		const multimap<std::string, std::string>& source) {

	pair<multimap<std::string, std::string>::const_iterator, multimap<std::string, std::string>::const_iterator> pairList;
	pairList = source.equal_range(key);

	int count = 0;

	// TODO: check iterator arithmetics...
	multimap<std::string, std::string>::const_iterator it;
	for (it = pairList.first; it != pairList.second; it++) {
		count++;
	}

	return count;
}

void CCmtsCmIfList::getIfList(const std::string& cm_mac, vector<std::string>& if_id_list) {
	this->getList(cm_mac, this->m_list_cm, if_id_list);
}

/**
 * Find a CM<->IF unique pair.
 * Useful to determine is there the pair.
 */
int CCmtsCmIfList::find(const std::string& cm_mac, const std::string& if_id) {
	return this->m_idx_cmifpair.getIdx(cm_mac, if_id);
}

int CCmtsCmIfList::find(CCmtsCm* pCm, CCmtsIf* pIf) {
	return this->find(pCm->getCmMac(), pIf->getIfId());
}


} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */



