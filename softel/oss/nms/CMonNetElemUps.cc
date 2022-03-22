/*
 * CMonNetElemUps.cc
 *
 *  Created on: 25.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CMonNetElemUps.h"

#include "CNetElemUps.h"
#include "CSqlNmsTools.h"
#include "CNetSnmpAsynchWrapper.h"

#include <softel/oss/cmts/CSnmpOid.h>
#include <softel/db/CDataRow.h>

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::CMTS;

namespace SF {
namespace OSS {
namespace NMS {

CMonNetElemUps::CMonNetElemUps() {
	// TODO Auto-generated constructor stub

}

CMonNetElemUps::~CMonNetElemUps() {
	// TODO Auto-generated destructor stub
}

void CMonNetElemUps::run() {

	std::cout << __FILE__ << "::" << __FUNCTION__ << std::endl;

	this->m_net_elem_list.dumpInfo();

	this->m_snmp_oid_list.dumpInfo();

	CNetSnmpAsynchWrapper snmpTool;

	vector<CSnmpOid> listSnmpOid;
	this->m_snmp_oid_list.getSnmpOidVector(listSnmpOid);

	snmpTool.snmpMultipleGetAsynch(this->m_net_elem_list, listSnmpOid, 1000);

	this->m_net_elem_list.dumpInfo();
}

/**
 * Initialize the UPS net elements list.
 * TODO: Now for testing it is here, but should be done from outer space...
 */
void CMonNetElemUps::init() {

	std::cout << __FILE__ << "::" << __FUNCTION__ << std::endl;

	CDataRow row;
	CDataSet ds;

	this->m_sql_tools.selectListNetElemUPS(ds);

	for(int i = 0; i < ds.size(); i++) {
		ds.getRow(i, row);
		this->m_net_elem_list.add(new CNetElemUps(row));
	}

	//this->m_net_elem_list.add(new CNetElemUps("192.168.193.253"));
}

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */



