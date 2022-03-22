/*
 * CAlertCmDedicated.cc
 *
 *  Created on: 24.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CAlertCmDedicated.h"

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::CMTS;

namespace SF {
namespace OSS {
namespace ALERT {

CAlertCmDedicated::CAlertCmDedicated(CCmtsCm* pCm)
	: CAlertCm(pCm) {

}

CAlertCmDedicated::~CAlertCmDedicated() {

}

/**
 * Save the Alert - should save everything into coresponding tables.
 * Check if the alert exists and update counter. Insert new Alert only if new one.
 */
void CAlertCmDedicated::saveAlert() {

	CDataSet ds;

	initAlert();

	m_dbTools.sqlSelectAlertCmDedicated(ds);

	if(ds.size() > 0) {

		CDataRow row;

		ds.getRow(0, row);

		m_param_set.setParam(PARAM_ALERT_ID, row.getColumn(PARAM_ALERT_ID));

		COssAlert::updateAlert();

	} else {

		doNewAlert();

	}

}

void CAlertCmDedicated::initAlert(CDataRow& row) {

	CAlertCm::initAlert(row);

	this->initAlert();
}

/**
 * Make a New Alert.
 */
void CAlertCmDedicated::doNewAlert() {

	CAlertCm::doNewAlert();

}

/**
 * Initialize alert params.
 */
void CAlertCmDedicated::initAlert() {
	setAlertType(TYPE_ALERT_CM_DEDICATED);
	setAlertSeverity(SEVERITY_ALERT_VHIGH);
}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */



