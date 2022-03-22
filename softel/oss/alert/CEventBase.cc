/*
 * CEventBase.cc
 *
 *  Created on: 13.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CEventBase.h"

using namespace std;
using namespace SF::OSS::CMTS;
using namespace SF::DB;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace OSS {
namespace ALERT {

CEventBase::CEventBase() {
	// TODO Auto-generated constructor stub

}

CEventBase::~CEventBase() {
	// TODO Auto-generated destructor stub
}

void CEventBase::init() {

	this->m_param_set.setParam(PARAM_EVENT_STATUS, 0);
	this->m_param_set.setParam(PARAM_EVENT_COUNTER, 1);
	this->m_param_set.setParam(PARAM_EVENT_DATE_INIT, CSystemTools::doGetCurrentTimeUx());
}

void CEventBase::setParam(const string& key, const string& val) {
	this->m_param_set.setParam(key, val);
}

string CEventBase::getParam(const string& key) {
	return m_param_set.getParam(key);
}

void CEventBase::setMessage(const string& message) {
	this->setParam(PARAM_EVENT_MESSAGE, message);
}

void CEventBase::dumpInfo() {

	this->m_param_set.dumpInfo();

}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */


