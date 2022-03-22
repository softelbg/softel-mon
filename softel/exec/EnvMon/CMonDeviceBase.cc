/*
 * CMonDeviceBase.cc
 *
 *  Created on: 12.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CMonDeviceBase.h"

using namespace std;
using namespace SF::OSS::EXEC;
using namespace SF::CORE::NET;

namespace SF {
namespace OSS {
namespace EXEC {

CMonDeviceBase::CMonDeviceBase() {
	// TODO Auto-generated constructor stub

}

CMonDeviceBase::CMonDeviceBase(const CHost& host) {
	this->setHost(host);
}

CMonDeviceBase::~CMonDeviceBase() {
	// TODO Auto-generated destructor stub
}

string CMonDeviceBase::getResponse() {
	return this->m_raw_response;
}

void CMonDeviceBase::setHost(const CHost& host) {
	this->m_host = host;
}

const CHost& CMonDeviceBase::getHost() const {
	return this->m_host;
}

void CMonDeviceBase::setRawResponse(const string& response) {
	this->m_raw_response = response;
}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */




