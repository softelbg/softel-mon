/*
 * CSnmpCmIp.cc
 *
 *  Created on: 19.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSnmpCmIp.h"

#include <softel/oss/cmts/CCmtsToolsSql.h>
#include <softel/core/util/CStringUtil.h>
#include <softel/core/net/CIp.h>
#include <softel/db/CMySqlDB.h>
#include <softel/db/CDataRow.h>

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::NET;
using namespace SF::CORE::UTIL;

namespace SF {
namespace OSS {
namespace SNMP {

CSnmpCmIp::CSnmpCmIp() {
	// TODO Auto-generated constructor stub

}

CSnmpCmIp::CSnmpCmIp(CDataRow& rowCmts) {

	this->init(rowCmts);
}

CSnmpCmIp::~CSnmpCmIp() {
	// TODO Auto-generated destructor stub
}

void CSnmpCmIp::init() {

	if (this->initCmtsInfo()) {
		this->initCmInfo();
	}

}

/**
 * Init from a CMTS Row (just copy the row).
 */
void CSnmpCmIp::init(CDataRow& rowCmts) {

	BSnmpCmtsBase::init(rowCmts);

	// TODO: Make some CMTS validation here....

	this->initCmInfo();

}

string CSnmpCmIp::getCmIp() {
	return this->getParam(KEY_COL_CM_IP);
}

/**
 * Read CPE IP from CMTS by CPE MAC
 */
bool CSnmpCmIp::initCmInfo() {

	string snmpResult;
	string snmpOid;

	CIp ipToolsCmts(this->getParam(KEY_COL_CMTS_IP),
					this->getParam(KEY_COL_CMTS_RCOMM));

	snmpOid = "1.3.6.1.4.1.9.9.116.1.3.1.1.3";

	string cmMac = this->getParam(KEY_SNMP_OID_CM_MAC_HEX);

	CStringUtil::convertMac2DottedDecimal(cmMac, ":", snmpOid);

	ipToolsCmts.SnmpGet(snmpOid, snmpResult);

	cout << __FUNCTION__ << " Result IP: " << snmpResult << endl;

	this->setParam(KEY_COL_CM_IP, snmpResult);

	return true;
}

} /* namespace SNMP */
} /* namespace OSS */
} /* namespace SF */




