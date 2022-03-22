/*
 * CCmtsCmLatestRaw.cc
 *
 *  Created on: 04.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCmtsCmLatestRaw.h"

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::CMTS;

namespace SF {
namespace OSS {
namespace ALERT {

CCmtsCmLatestRaw::CCmtsCmLatestRaw() {
	// TODO Auto-generated constructor stub

}

CCmtsCmLatestRaw::~CCmtsCmLatestRaw() {
	// TODO Auto-generated destructor stub
}

/**
 * Read latest cm values
 */
void CCmtsCmLatestRaw::init() {

	CSqlThresholdTools sqlTools;

	sqlTools.selectCmLatest(this->m_data_set);

}

/**
 * Compare full CM list with full Oid Threshold list.
 */
void CCmtsCmLatestRaw::compare(CThresholdOidList& listOidThresholds) {

	cout << __FILE__ << ":" << __FUNCTION__ << " m_data_set.size:" << m_data_set.size();
	cout << " listOidThresholds.size: " << listOidThresholds.size() << endl;

	for(int i = 0; i < this->m_data_set.size(); i++) {

		CDataRow row;

		this->m_data_set.getRow(i, row);

		listOidThresholds.compareCmRaw(row, this->m_list_compared_raw_cm_thresults);

	}

}

const CDataSet& CCmtsCmLatestRaw::getRawDataSet() const {
	return this->m_data_set;
}

const map<string, vector<CThresholdResult*> >& CCmtsCmLatestRaw::getRawCmThresholdResults() {
	return this->m_list_compared_raw_cm_thresults;
}

const vector<CThresholdResult*>& CCmtsCmLatestRaw::getRawCmThresholdResults(
		const string& severity) {

	map<string, vector<CThresholdResult*> >::const_iterator it;

	it = this->m_list_compared_raw_cm_thresults.find(severity);

	if (it != this->m_list_compared_raw_cm_thresults.end()) {
		return it->second;
	} else {
		return this->m_list_compared_raw_cm_thresults.begin()->second;
	}

}

void CCmtsCmLatestRaw::dumpInfo() const {

	//this->m_data_set.dumpInfo();

	map<string, vector<CThresholdResult*> >::const_iterator it;
	for (it = this->m_list_compared_raw_cm_thresults.begin();
			it != this->m_list_compared_raw_cm_thresults.end(); it++) {

		for (int i = 0; i < it->second.size(); i++) {

			cout << "M[" << it->first << "]";
			cout << " cm_mac:" << it->second[i]->getRow().getColumn("cm_mac");
			cout << " " << it->second[i]->getParam(KEY_PARAM_COL_KEY);
			cout << " = " << it->second[i]->getParam(PARAM_THRES_VALUE);
			cout << " color: " << it->second[i]->getParam(PARAM_THRES_COLOR);

			cout << endl;

		}

	}
}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */



