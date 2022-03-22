/*
 * COssAlert.cc
 *
 *  Created on: 24.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "COssAlert.h"

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace OSS {
namespace ALERT {

COssAlert::COssAlert() {

	initAlert();

	m_dbTools.setAlert(&m_param_set);

}

COssAlert::COssAlert(CMySqlDB* pDb) {

	m_pDb = pDb;

	initAlertFromDb();

	m_dbTools.setAlert(&m_param_set);

}

COssAlert::COssAlert(CDataRow& row) {

	initAlert(row);

	m_dbTools.setAlert(&m_param_set);

}

COssAlert::~COssAlert() {

}

void COssAlert::setAlertType(int type) {
	m_param_set.setParam(PARAM_ALERT_TYPE, type);
}

void COssAlert::setAlertStatus(int status) {
	m_param_set.setParam(PARAM_ALERT_STATUS, status);
}

/**
 * Set CLOSED Alert Status.
 * TODO: do status define list...
 */
void COssAlert::setAlertStatusClosed() {
	setAlertStatus(3);
}

void COssAlert::setAlertSeverity(int severity) {
	m_param_set.setParam(PARAM_ALERT_SEVERITY, severity);
}

void COssAlert::setAlertCounter(int count) {
	m_param_set.setParam(PARAM_ALERT_COUNTER, count);
}

void COssAlert::setAlertDateInit(long uxdate) {
	m_param_set.setParam(PARAM_ALERT_DATE_INIT, uxdate);
}

string COssAlert::getAlertType() {
	return m_param_set.getParam(PARAM_ALERT_TYPE);
}

string COssAlert::getAlertStatus() {
	return m_param_set.getParam(PARAM_ALERT_STATUS);
}

string COssAlert::getAlertSeverity() {
	return m_param_set.getParam(PARAM_ALERT_SEVERITY);
}

string COssAlert::getAlertCounter() {
	return m_param_set.getParam(PARAM_ALERT_COUNTER);
}

string COssAlert::getAlertDateInit() {
	return m_param_set.getParam(PARAM_ALERT_DATE_INIT);
}

/**
 * Initialize Alert from DB
 */
void COssAlert::initAlertFromDb() {

}

/**
 * Initialize Alert from a Data Row.
 *
 * TODO: Consider automation of param list.
 */
void COssAlert::initAlert(CDataRow& row) {

	/*
	vector<string> listParams;

	listParams.push_back(PARAM_ALERT_ID);
	listParams.push_back(PARAM_ALERT_TYPE);
	listParams.push_back(PARAM_ALERT_STATUS);
	listParams.push_back(PARAM_ALERT_COUNTER);
	listParams.push_back(PARAM_ALERT_DATE_INIT);
	listParams.push_back(PARAM_ALERT_SEVERITY);
	listParams.push_back(PARAM_ALERT_MESSAGE);

	for(unsigned int i = 0; i < listParams.size(); i++) {
		m_param_set.setParam(listParams[i], row.getColumn(listParams[i]));
	}
*/

	this->m_param_set.clear();

	for(int i = 0; i < row.size(); i++) {

		string key;
		string val;

		row.getColumn(i, key, val);

		this->m_param_set.setParam(key, val);
	}

	initDateCurrent();

}

void COssAlert::initAlert() {

	initAlertStatus();
	initAlertCounter();
	initAlertSeverity();
	initDate();

}

/**
 * Save new Alert into DB.
 *
 *
 */
void COssAlert::saveAlert() {

	COssAlert::doNewAlert();

}

/**
 * Update the Alert - increase counter, update some params...
 */
void COssAlert::updateAlert() {

	m_dbTools.sqlIncAlertBase();

}

/**
 * Set initial status value = new
 */
void COssAlert::initAlertStatus() {
	setAlertStatus(0);
}

/**
 * Set initial Counter value = 1
 */
void COssAlert::initAlertCounter() {
	setAlertCounter(1);
}

/**
 * Set most urgent severity
 */
void COssAlert::initAlertSeverity() {
	setAlertSeverity(0);
}

/**
 * Set inital unix date to now
 */
void COssAlert::initDate() {
	setAlertDateInit(CSystemTools::doGetCurrentTimeUx());
}

void COssAlert::setAlertMessage(const string& message) {
	this->setParam(PARAM_ALERT_MESSAGE, message);
}

/**
 * Set current unix date to now
 */
void COssAlert::initDateCurrent() {
	m_param_set.setParam(PARAM_ALERT_DATE, CSystemTools::doGetCurrentTimeUx());
}

/**
 * Make a New Alert.
 */
void COssAlert::doNewAlert() {

	m_dbTools.sqlInsertAlertBase();

}

/**
 * Close an Alert.
 */
void COssAlert::doCloseAlert() {

	setAlertStatusClosed();

	initDateCurrent();

	m_dbTools.sqlUpdateAlertBase();
}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */


