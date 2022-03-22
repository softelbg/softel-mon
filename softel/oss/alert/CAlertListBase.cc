/*
 * CAlertListBase.cc
 *
 *  Created on: 06.11.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CAlertListBase.h"

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::CMTS;

namespace SF {
namespace OSS {
namespace ALERT {

CAlertListBase::CAlertListBase() {
	// TODO Auto-generated constructor stub

}

CAlertListBase::~CAlertListBase() {
	// TODO Auto-generated destructor stub
}

void CAlertListBase::clear() {
	this->m_pAlertList.clear();
	this->m_index.clear();
}

size_t CAlertListBase::size() const {
	return this->m_pAlertList.size();
}

void CAlertListBase::add(COssAlert* pAlert) {
	this->m_pAlertList.push_back(pAlert);
}

void CAlertListBase::add(const string& key, COssAlert* pAlert) {
	this->m_index.setIdx(key, this->size());
	this->add(pAlert);
}

COssAlert* CAlertListBase::getAlert(size_t pos) {
	if (pos >=0 && pos < this->size()) {
		return this->m_pAlertList[pos];
	} else {
		return NULL;
	}
}

COssAlert* CAlertListBase::getAlert(const string& key) {
	return this->getAlert(this->m_index.getIdx(key));
}

void CAlertListBase::dumpInfo() {

	cout << __FILE__ << ": alert size:" << this->size() << endl;

	COssAlert* pAlert;

	for(int i = 0; i < this->size(); i++) {

		pAlert = this->getAlert(i);

		if (pAlert) {
			cout << "Alert[" << i << "]" << endl;
			pAlert->dumpInfo();
			cout << "------------------" << endl;
		}

	}
}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */



