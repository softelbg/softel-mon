/*
 * CAlertCmtsIf.cc
 *
 *  Created on: 24.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CAlertCmtsIf.h"

using namespace std;
using namespace SF::OSS::CMTS;
using namespace SF::DB;

namespace SF {
namespace OSS {
namespace ALERT {

CAlertCmtsIf::CAlertCmtsIf() {

}

CAlertCmtsIf::CAlertCmtsIf(const CCmtsIf& cmtsIf) {

	this->setParam(KEY_COL_CMTS_ID, cmtsIf.getCmtsId());
	this->setParam(KEY_COL_IF_ID, cmtsIf.getIfId());
	this->setParam(PARAM_EVENT_MESSAGE, cmtsIf.getParam(KEY_PARAM_EVENT_MESSAGE));
	this->setParam(PARAM_ALERT_MESSAGE, cmtsIf.getParam(KEY_PARAM_EVENT_MESSAGE));
}

CAlertCmtsIf::CAlertCmtsIf(const std::string& cmts_id, const std::string& if_id) {

	this->setParam(KEY_COL_CMTS_ID, cmts_id);
	this->setParam(KEY_COL_IF_ID, if_id);
}

CAlertCmtsIf::~CAlertCmtsIf() {

}

void CAlertCmtsIf::initAlert() {
	setAlertType(TYPE_ALERT_CMTS_IF);
	setAlertSeverity(SEVERITY_ALERT_MIDDLE);
}

void CAlertCmtsIf::initAlert(CDataRow& row) {

	COssAlert::initAlert(row);

	this->initAlert();
}

void CAlertCmtsIf::saveAlert() {
}

void CAlertCmtsIf::doNewAlert() {

	COssAlert::saveAlert();

	m_dbTools.sqlInsertAlertIf();
}

void CAlertCmtsIf::initParams() {
}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */




