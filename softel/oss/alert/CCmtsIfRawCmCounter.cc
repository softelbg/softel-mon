/*
 * CCmtsIfRawCmCounter.cc
 *
 *  Created on: 27.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 *
 */

#include "CCmtsIfRawCmCounter.h"

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::CMTS;

namespace SF {
namespace OSS {
namespace ALERT {

CCmtsIfRawCmCounter::CCmtsIfRawCmCounter() {

	this->init();

}

CCmtsIfRawCmCounter::~CCmtsIfRawCmCounter() {
	// TODO Auto-generated destructor stub
}

void CCmtsIfRawCmCounter::clear() {
	this->m_count_cm.clear();
}

void CCmtsIfRawCmCounter::init() {

	CDataRow row;
	CSqlAlertTools sqlTools;

	sqlTools.sqlSelectCmIfDocs3List(this->m_docs3_interfaces);

	for(int i = 0; i < this->m_docs3_interfaces.size(); i++) {

		this->m_docs3_interfaces.getRow(i, row);

		string cm_mac;
		string if_id;

		row.getColumn("cm_mac", cm_mac);
		row.getColumn("if_id", if_id);

		//this->m_cm_if_list[cm_mac] = if_id;
		this->m_cm_if_list.insert(pair<string, string>(cm_mac, if_id));

	}
}

size_t CCmtsIfRawCmCounter::size() const {
	return this->m_count_cm.size();
}

/**
 * Return size of Interfaces map for a CMTS
 */
size_t CCmtsIfRawCmCounter::size(const string& cmts_id) const {

	map<string, map<string, int> >::const_iterator it;

	it = this->m_count_cm.find(cmts_id);

	if (it != this->m_count_cm.end()) {
		return it->second.size();
	} else {
		return 0;
	}

}

/**
 * Add a Raw CM as incrementing respective CMTS IFs counters.
 */
void CCmtsIfRawCmCounter::add(const CDataRow& rowCmRaw) {

	map<string, string> current_cm_if_list;

	string cm_mac;
	string cmts_id;
	string if_up;
	string if_dwn;

	rowCmRaw.getColumn("cm_mac", cm_mac);
	rowCmRaw.getColumn("cmts_id", cmts_id);
	rowCmRaw.getColumn("if_up", if_up);
	rowCmRaw.getColumn("if_dwn", if_dwn);

	pair<multimap<string, string>::iterator, multimap<string, string>::iterator> pairCmIf;
	pairCmIf = this->m_cm_if_list.equal_range(cm_mac);

	current_cm_if_list[if_up] = if_up;
	current_cm_if_list[if_dwn] = if_dwn;

	multimap<string, string>::iterator it;
	for(it = pairCmIf.first; it != pairCmIf.second; it++) {
		current_cm_if_list[it->second] = it->second;
	}

	this->increment(cmts_id, current_cm_if_list);

}

/**
 * Increment count for CMs cmts->if
 */
void CCmtsIfRawCmCounter::increment(const string& cmts_id, const string& if_id) {

	int count = this->m_count_cm[cmts_id][if_id];

	count++;

	this->m_count_cm[cmts_id][if_id] = count;

}

void CCmtsIfRawCmCounter::increment(const string& cmts_id, const vector<string>& if_id_list) {

	for(unsigned int i = 0; i < if_id_list.size(); i++) {
		this->increment(cmts_id, if_id_list[i]);
	}

}

const map<string, map<string, int> >& CCmtsIfRawCmCounter::getRawCmCount() const {
	return this->m_count_cm;
}

/**
 * Get Map with IF->cm counts for a CMTS
 */
const map<string, int>& CCmtsIfRawCmCounter::getRawCmCount(
		const string& cmts_id) const {

	map<string, map<string, int> >::const_iterator it;

	it = this->m_count_cm.find(cmts_id);

	if (it != this->m_count_cm.end()) {
		return it->second;
	} else {
		return this->m_count_cm.begin()->second;
	}
}

/**
 * Get CMTS:IF CM count.
 */
const int CCmtsIfRawCmCounter::getRawCmCount(const string& cmts_id,
		const string& if_id) const {

	int result;

	map<string, map<string, int> >::const_iterator it;

	it = this->m_count_cm.find(cmts_id);

	if (it != this->m_count_cm.end()) {

		map<string, int>::const_iterator it2;

		it2 = it->second.find(if_id);

		if (it2 != it->second.end()) {
			result = it2->second;
		} else {
			result = -1;
		}

	} else {
		result = -1;
	}

	return result;
}

void CCmtsIfRawCmCounter::increment(const string& cmts_id, const map<string, string>& if_id_list) {

	map<string, string>::const_iterator it;

	for(it = if_id_list.begin(); it != if_id_list.end(); it++) {
		this->increment(cmts_id, it->first);
	}
}

void CCmtsIfRawCmCounter::dumpInfo() {

	map<string, map<string, int> >::iterator it1;

	for(it1 = this->m_count_cm.begin(); it1 != this->m_count_cm.end(); it1++) {

		cout << "CMTS[" << it1->first << "]" << endl;

		map<string, int>::iterator it2;

		for(it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {

			cout << "\t\t" << "IF[" << it2->first << "] count = " << it2->second << endl;

		}

	}

}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */




