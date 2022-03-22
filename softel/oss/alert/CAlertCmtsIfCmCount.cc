/*
 * CAlertCmtsIfCmCount.cc
 *
 *  Created on: 06.11.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CAlertCmtsIfCmCount.h"

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::CMTS;

namespace SF {
namespace OSS {
namespace ALERT {

CAlertCmtsIfCmCount::CAlertCmtsIfCmCount(const string& cmts_id, const string& if_id) :
	CAlertCmtsIf(cmts_id, if_id) {

}

CAlertCmtsIfCmCount::CAlertCmtsIfCmCount(const CCmtsIf& cmtsIf) :
	CAlertCmtsIf(cmtsIf) {

	this->setAlertMessage(cmtsIf.getParam(KEY_PARAM_EVENT_MESSAGE));
}

CAlertCmtsIfCmCount::~CAlertCmtsIfCmCount() {
	// TODO Auto-generated destructor stub
}

void CAlertCmtsIfCmCount::init(const CCmtsIfCmCounter& ifCmCounter) {
}

void CAlertCmtsIfCmCount::initAlert() {
	setAlertType(TYPE_ALERT_CMTS_IF_COUNTER);
	setAlertSeverity(SEVERITY_ALERT_HIGH);
}

void CAlertCmtsIfCmCount::initAlert(CDataRow& row) {
	CAlertCmtsIf::initAlert(row);
}

/**
 * Save the Alert - should save everything into coresponding tables.
 * Check if the alert exists and update counter. Insert new Alert only if new one.
 */
void CAlertCmtsIfCmCount::saveAlert() {

	CDataSet ds;

	initAlert();

	m_dbTools.sqlSelectAlertCmtsIfCounter(ds);

	if (ds.size() > 0) {

		CDataRow row;

		ds.getRow(0, row);

		m_param_set.setParam(PARAM_ALERT_ID, row.getColumn(PARAM_ALERT_ID));

		COssAlert::updateAlert();

	} else {

		doNewAlert();

	}
}

void CAlertCmtsIfCmCount::doNewAlert() {
	CAlertCmtsIf::doNewAlert();
}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */


