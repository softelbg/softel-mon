/*
 * BNetworkElementBase.cc
 *
 *  Created on: 21.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BNetworkElementBase.h"

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::CMTS;

namespace SF {
namespace OSS {
namespace NMS {

BNetworkElementBase::BNetworkElementBase() {
	// TODO Auto-generated constructor stub

}

BNetworkElementBase::BNetworkElementBase(const SF::DB::CDataRow& row) {

	row.copyRow(this->m_row_params);

	this->addIf(new CNetworkElementIF(row));

}

BNetworkElementBase::~BNetworkElementBase() {
	// TODO Auto-generated destructor stub
}

void BNetworkElementBase::addIf(CNetworkElementIF* net_if) {
	this->m_if_list.addIf(net_if);
}

CNetworkElementIFList& BNetworkElementBase::getIfList() {
	return this->m_if_list;
}

CSnmpValueSet& BNetworkElementBase::getSnmpValueSet() {
	return this->m_value_set;
}

/**
 * Return IP of main Interface which should be used to be queried via SNMP.
 *
 * TODO: Now it returns first validated interface...
 */
string BNetworkElementBase::getIp() const {

	string ip;

	for(int i = 0; i < this->m_if_list.size(); i++) {

		if (this->m_if_list.getIf(0)->isValid()) {
			ip = this->m_if_list.getIf(0)->getIp();
			break;
		}

	}

	return ip;
}

/**
 * Make a index key list from some of the properties
 */
const vector<string> BNetworkElementBase::getIdxKeyList() const {

	vector<string> listKeys;

	listKeys.push_back(this->getIp());

	return listKeys;
}

string BNetworkElementBase::getSnmpRComm() const {
	return this->m_row_params.getColumn(OSS_NET_ELEM_SNMP_RCOMM);
}

string BNetworkElementBase::getSnmpVer() const {
	return this->m_row_params.getColumn(OSS_NET_ELEM_SNMP_VER);
}

/**
 * Check is the NetElem Active - using various params to determinte activity.
 */
bool BNetworkElementBase::isActive() const {
	return true;
}

void BNetworkElementBase::dumpInfo() {

	cout << "NetworkElement ---------------------" << endl;

	this->m_row_params.dumpInfo();
	this->m_if_list.dumpInfo();
	this->m_value_set.dumpInfo();

	cout << "--------------------------------------" << endl;
}

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */


