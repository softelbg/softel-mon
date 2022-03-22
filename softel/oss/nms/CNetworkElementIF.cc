/*
 * CNetworkElementIF.cc
 *
 *  Created on: 21.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CNetworkElementIF.h"

using namespace std;
using namespace SF::DB;

namespace SF {
namespace OSS {
namespace NMS {

CNetworkElementIF::CNetworkElementIF() {
	// TODO Auto-generated constructor stub

}

CNetworkElementIF::CNetworkElementIF(const string& ip) {
	this->setIp(ip);
}

CNetworkElementIF::CNetworkElementIF(const string& ip, const string& mac) {
	this->setIp(ip);
	this->setMac(mac);
}

CNetworkElementIF::CNetworkElementIF(const CDataRow& row) {
	row.copyRow(this->m_row_params);
}

CNetworkElementIF::~CNetworkElementIF() {
	// TODO Auto-generated destructor stub
}

void CNetworkElementIF::setIp(const string& ip) {
	this->m_row_params.addColumn(OSS_NET_IP, ip);
}

string CNetworkElementIF::getIp() const {
	return this->m_row_params.getColumn(OSS_NET_IP);
}

void CNetworkElementIF::setMac(const string& mac) {
	this->m_row_params.addColumn(OSS_NET_MAC, mac);
}

string CNetworkElementIF::getMac() const {
	return this->m_row_params.getColumn(OSS_NET_MAC);
}

/**
 * REturn a list of keys from some content of the component
 */
const vector<string> CNetworkElementIF::getIdxKeyList() const {

	vector<string> listKeys;

	if (this->getIp().size() > 0) {
		listKeys.push_back(this->getIp());
	}

	if (this->getMac().size() > 0) {
		listKeys.push_back(this->getMac());
	}

	return listKeys;
}

/**
 * Validate the NetElem Interface
 * Suppose at least there should be an valid IP address.
 * TODO: Make validation better...
 */
bool CNetworkElementIF::isValid() {

	if (this->getIp().size() > 0) {
		return true;
	}

	return false;
}

void CNetworkElementIF::dumpInfo() const {

	cout << "ip:" << this->getIp() << " mac:" << this->getMac() << endl;

}

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */





