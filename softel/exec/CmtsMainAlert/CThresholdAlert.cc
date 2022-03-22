/*
 * CThresholdAlert.cc
 *
 *  Created on: 03.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CThresholdAlert.h"

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::ALERT;

namespace SF {
namespace OSS {
namespace EXEC {

CThresholdAlert::CThresholdAlert() {

}

CThresholdAlert::~CThresholdAlert() {
	// TODO Auto-generated destructor stub
}

void CThresholdAlert::start() {

	this->m_threshold_list_raw_cm.init();
	this->m_threshold_list_raw_if.init();

	CThresholdOidList thresholdOidListCm(this->m_threshold_list_raw_cm.getRawDataSet());
	//thresholdOidListCm.dumpInfo();

	CCmtsCmLatestRaw cmRawList;
	cmRawList.init();
	cmRawList.compare(thresholdOidListCm);

	//cmRawList.dumpInfo();

	// TODO: Testing CM Counters per IF
	CCmtsIfRawCmCounter cmAll;
	CCmtsIfRawCmCounter cmBad;

	CDataRow row;

	for(int i = 0; i < cmRawList.getRawDataSet().size(); i++) {
		cmRawList.getRawDataSet().getRow(i, row);
		cmAll.add(row);
	}

	map<string, CDataRow> list_bad_cm;
	const vector<CThresholdResult*>& listThresholdResults = cmRawList.getRawCmThresholdResults("0");
	for(size_t i = 0; i < listThresholdResults.size(); i++) {
		const CDataRow& rowCurrent = listThresholdResults[i]->getRow();
		list_bad_cm[rowCurrent.getColumn("cm_mac")] = rowCurrent;
	}

	map<string, CDataRow>::const_iterator it;
	for(it = list_bad_cm.begin(); it != list_bad_cm.end(); it++) {
		cmBad.add(it->second);
	}

	CCmtsIfCmCounter cmPartialCounter(&cmAll, &cmBad);

	/*
	cout << "START cmAll" << endl;
	cmAll.dumpInfo();
	cout << "END cmAll START cmBad" << endl;
	cmBad.dumpInfo();
	cout << "END cmBad" << endl;

	cmPartialCounter.dumpInfo();
*/

	CAlertListCmtsIfCmCount listAlertsCount(cmPartialCounter);
	//listAlertsCount.dumpInfo();
	listAlertsCount.save();

}

void CThresholdAlert::dumpInfo() const {

	this->m_threshold_list_raw_cm.dumpInfo();

	this->m_threshold_list_raw_if.dumpInfo();

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */


