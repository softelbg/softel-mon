/*
 * CSnmpValueSpeed.cc
 *
 *  Created on: 17.08.2012
 *      Author: STAN
 *
 *		www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSnmpValueSpeed.h"

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::NET;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace OSS {
namespace CMTS {

CSnmpValueSpeed::CSnmpValueSpeed(const CSnmpOid& snmpOid) :
	CSnmpValue(snmpOid) {

	this->m_values.clear();

}

CSnmpValueSpeed::~CSnmpValueSpeed() {
	// TODO Auto-generated destructor stub
}

/**
 * Return string with Formated Speed
 */
string& CSnmpValueSpeed::getSpeed() {

	stringstream ss;

	ss << this->getSnmpOid().getProperty(KEY_PARAM_OID_LABEL);
	ss << " " << this->getValue();
	ss << " " << this->getSnmpOid().getProperty(KEY_PARAM_OID_DIM) << "/s";

	this->m_speed = ss.str();

	return this->m_speed;
}

void CSnmpValueSpeed::setIpTools(CIp* pIpTools) {
	this->m_pIpTools = pIpTools;
}

void CSnmpValueSpeed::setPeriod(int period) {
	this->m_period = period;
}

void CSnmpValueSpeed::start() {

	string snmpResult;
	unsigned int i;

	// Check is it already started
	if (this->m_values.size() > 0) {
		return;
	}

	this->m_counter.reset();

	CSnmpOid oid = this->getSnmpOid();

	m_pIpTools->SnmpGet(this->getSnmpOid().getSnmpOid(), snmpResult);
	oid.addProperty(KEY_PARAM_OID_ID, "id:0");
	m_values.setSnmpValue(oid, snmpResult);

	CSystemTools::doSleep(this->m_period);

	m_pIpTools->SnmpGet(this->getSnmpOid().getSnmpOid(), snmpResult);
	oid.addProperty(KEY_PARAM_OID_ID, "id:1");
	m_values.setSnmpValue(oid, snmpResult);

	this->m_counter.stop();

	double period = this->m_counter.getTimeElapsed();

	float  resultSpeed = m_values.getSnmpValue(1)->getValueFloat() - m_values.getSnmpValue(0)->getValueFloat();

	// Check for counter overflow error
	if (resultSpeed < 0) {
		resultSpeed = m_values.getSnmpValue(1)->getValueFloat() + ( 0xffffffff - m_values.getSnmpValue(0)->getValueFloat());
	}

	resultSpeed = resultSpeed / period;

	this->setSnmpValueParsed(CStringUtil::fromDouble(resultSpeed));
}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */
