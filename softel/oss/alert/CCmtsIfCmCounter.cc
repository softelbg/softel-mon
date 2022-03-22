/*
 * CCmtsIfCmCounter.cc
 *
 *  Created on: 05.11.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCmtsIfCmCounter.h"

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::CMTS;

namespace SF {
namespace OSS {
namespace ALERT {

CCmtsIfCmCounter::CCmtsIfCmCounter(CCmtsIfRawCmCounter* pAll,
		CCmtsIfRawCmCounter* pPartial) {

	this->m_pAll = pAll;
	this->m_pPartial = pPartial;

	this->init();
}

CCmtsIfCmCounter::~CCmtsIfCmCounter() {
	// TODO Auto-generated destructor stub
}

void CCmtsIfCmCounter::init() {

	this->clear();

	if(this->m_pAll == NULL || this->m_pPartial == NULL) {
		return;
	}

	const map<string, map<string, int> >& listAll = this->m_pAll->getRawCmCount();
	const map<string, map<string, int> >& listPartial = this->m_pPartial->getRawCmCount();

	map<string, map<string, int> >::const_iterator it1;

	for (it1 = listPartial.begin(); it1 != listPartial.end(); it1++) {

		map<string, int>::const_iterator it2;

		for (it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {

			map<string, map<string, int> >::const_iterator it21;
			map<string, int>::const_iterator it22;

			int countPartial = it2->second;

			it21 = listAll.find(it1->first);
			if (it21 != listAll.end()) {

				it22 = it21->second.find(it2->first);

				if (it22 != it21->second.end()) {

					int countAll = it22->second;

					if (countAll > 0) {

						double result = (countPartial * 100) / countAll;

						this->m_calculated[it1->first][it2->first] = result;

					}
				}
			}

		}

	}
}

void CCmtsIfCmCounter::clear() {
	this->m_calculated.clear();
}

size_t CCmtsIfCmCounter::size() const {
	return this->m_calculated.size();
}

size_t CCmtsIfCmCounter::size(const string& cmts_id) const {

	map<string, map<string, double> >::const_iterator it;

	it = this->m_calculated.find(cmts_id);

	if(it != this->m_calculated.end()) {
		return it->second.size();
	} else {
		return -1;
	}

}

const map<string, map<string, double> >& CCmtsIfCmCounter::getCalculated() const {
	return this->m_calculated;
}

const map<string, double>& CCmtsIfCmCounter::getCalculated(
		const string& cmts_id) const {

	map<string, map<string, double> >::const_iterator it;

	it = this->m_calculated.find(cmts_id);

	if (it != this->m_calculated.end()) {
		return it->second;
	} else {
		return this->m_calculated.begin()->second;
	}
}

const double CCmtsIfCmCounter::getCalculated(const string& cmts_id,
		const string& if_id) const {

	const map<string, double>& cmtsIfCmList = this->getCalculated(cmts_id);

	map<string, double>::const_iterator it;

	it = cmtsIfCmList.find(if_id);

	if (it != cmtsIfCmList.end()) {
		return it->second;
	} else {
		return -1;
	}
}

bool CCmtsIfCmCounter::getCounters(const string& cmts_id, const string& if_id,
		map<string, int>& result) const {

	double calc = this->getCalculated(cmts_id, if_id);
	int all = this->m_pAll->getRawCmCount(cmts_id, if_id);
	int partial = this->m_pPartial->getRawCmCount(cmts_id, if_id);

	if (calc != -1 && all != -1 && partial != -1) {
		result["calc"] = (int) calc;
		result["all"] = all;
		result["partial"] = partial;
	} else {
		return false;
	}

	return true;
}

void CCmtsIfCmCounter::dumpInfo() {

	map<string, map<string, double> >::iterator it1;

	for (it1 = this->m_calculated.begin(); it1 != this->m_calculated.end(); it1++) {

		cout << "CMTS[" << it1->first << "]" << endl;

		map<string, double>::iterator it2;

		for (it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {

			cout << "\t\t" << "IF[" << it2->first << "] = " << it2->second << " % ";
			cout << "all:" << this->m_pAll->getRawCmCount(it1->first, it2->first);
			cout << " partial:" << this->m_pPartial->getRawCmCount(it1->first, it2->first);
			cout << endl;
		}

	}

}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */


