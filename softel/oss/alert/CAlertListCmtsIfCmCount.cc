/*
 * CAlertListCmtsIfCmCount.cc
 *
 *	Alert List based on CmtsIfCm counts.
 *	Check the count of bad CMs. Usually arround 20% is threshold for bad/all.
 *	But should consider the all CM number - if it is too low an alert should not be made.
 *
 *  Created on: 06.11.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CAlertListCmtsIfCmCount.h"

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::CMTS;

namespace SF {
namespace OSS {
namespace ALERT {

CAlertListCmtsIfCmCount::CAlertListCmtsIfCmCount() {
	// TODO Auto-generated constructor stub

}

CAlertListCmtsIfCmCount::CAlertListCmtsIfCmCount(const CCmtsIfCmCounter& counter) {

	this->init(counter);

}

CAlertListCmtsIfCmCount::~CAlertListCmtsIfCmCount() {
	// TODO Auto-generated destructor stub
}

/**
 * Initialize Alert List from a Cmts If->CM Counter list.
 */
void CAlertListCmtsIfCmCount::init(const CCmtsIfCmCounter& counter) {

	this->m_list_good_if.clear();

	const map<string, map<string, double> >& listCmtsIfCmCalculated = counter.getCalculated();

	map<string, map<string, double> >::const_iterator it;

	for(it = listCmtsIfCmCalculated.begin(); it != listCmtsIfCmCalculated.end(); it++) {

		map<string, double>::const_iterator it2;

		for(it2 = it->second.begin(); it2 != it->second.end(); it2++) {

			map<string, int> listCounters;

			counter.getCounters(it->first, it2->first, listCounters);

			this->addAlert(it->first, it2->first, listCounters);

		}

	}

}

/**
 * Save all Alerts in the list.
 * Isnert/Update Alerted IFs.
 * Close all NON Alerted IFs if there were open Alerts.
 *
 */
void CAlertListCmtsIfCmCount::save() {

	COssAlert* pAlert;

	// Close already opened alerts for good IFs
	this->closeAlerts();

	for (int i = 0; i < this->size(); i++) {

		pAlert = this->getAlert(i);

		if (pAlert) {

			pAlert->saveAlert();

		}

	}

}

/**
 * Add a Alert for a cmts:if:counters if conditions are met.
 */
void CAlertListCmtsIfCmCount::addAlert(const string& cmts_id,
		const string& if_id, const map<string, int>& listCounters) {

	int minCmCount = 25;
	int maxCalculated = 20;

	int calc = 0;
	int all = 0;
	int partial = 0;

	map<string, int>::const_iterator it;

	it = listCounters.find("calc");
	if (it != listCounters.end()) {
		calc = it->second;
	}

	it = listCounters.find("all");
	if (it != listCounters.end()) {
		all = it->second;
	}

	it = listCounters.find("partial");
	if (it != listCounters.end()) {
		partial = it->second;
	}

	CCmtsIf cmtsIf(cmts_id, if_id);

	if (all > minCmCount && calc >= maxCalculated) {

		stringstream message;
		message << "Problem Cable Modems: " << calc << "%";

		cmtsIf.addEventMessage(message.str());

		this->m_list_alert_if.push_back(cmtsIf);

		COssAlert* pAlert = new CAlertCmtsIfCmCount(cmtsIf);

		this->add(pAlert);

	} else {
		this->m_list_good_if.push_back(cmtsIf);
	}

}

/**
 * Close all Good IFs Opne Alerts.
 * Automatic close of already non active alerts.
 */
void CAlertListCmtsIfCmCount::closeAlerts() {

	CDataSet dsIfAlert;

	CSqlAlertTools sqlTools;

	sqlTools.sqlSelectAlertCmtsIfCounterList(dsIfAlert);

	dsIfAlert.buildColumnIndex(KEY_COL_CMTS_ID, KEY_COL_IF_ID);

	// Close Alert for every good IF if there is an open one.
	for(int i = 0; i < this->m_list_good_if.size(); i++) {

		CDataRow row;

		int pos = dsIfAlert.searchIndexedColumn(KEY_COL_CMTS_ID,
												KEY_COL_IF_ID,
												this->m_list_good_if[i].getCmtsId(),
												this->m_list_good_if[i].getIfId());

		if (pos >= 0) {

			dsIfAlert.getRow(pos, row);

			CAlertCmtsIfCmCount ifAlert(this->m_list_good_if[i]);
			ifAlert.initAlert(row);
			ifAlert.doCloseAlert();

		}

	}
}


} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */




