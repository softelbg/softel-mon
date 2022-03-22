/*
 * BSnmpOidListBase.cc
 *
 *  Created on: 04.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BSnmpOidListBase.h"

#include <softel/core/system/CSystemTools.h>

using namespace std;
using namespace SF::OSS::CMTS;
using namespace SF::DB;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace OSS {
namespace NMS {

BSnmpOidListBase::BSnmpOidListBase() {

	this->init();

}

BSnmpOidListBase::~BSnmpOidListBase() {
	this->clear();
}

void BSnmpOidListBase::clear() {

	for(int i = 0; i < this->size(); i++) {
		delete this->get(i);
	}

	this->m_oid_list.clear();
	this->m_idx.clear();
}

size_t BSnmpOidListBase::size() const {
	return this->m_oid_list.size();
}

/**
 * Add new SnmpOid and create an index for it.
 */
void BSnmpOidListBase::add(BSnmpOidBase* pSnmpOid) {

	if (pSnmpOid) {
		this->addIdx(pSnmpOid);
		this->m_oid_list.push_back(pSnmpOid);
	}

}

BSnmpOidBase* BSnmpOidListBase::get(size_t pos) {

	BSnmpOidBase* pSnmpOid = NULL;

	if (pos >= 0 && pos < this->size()) {
		pSnmpOid = this->m_oid_list[pos];
	}

	return pSnmpOid;
}

BSnmpOidBase* BSnmpOidListBase::get(const string& key) {
	return this->get(this->m_idx.getIdx(key));
}

/**
 * Initialize the SnmpOid List.
 */
void BSnmpOidListBase::init() {

	CDataSet ds;

	CSqlNmsTools sqlTools;

	sqlTools.selectListSnmpOidNetElemBase(ds);

	this->init(ds);

}

/**
 * Initialize the SnmpOid List from a complete DataSet
 */
void BSnmpOidListBase::init(const CDataSet& ds) {

	CDataRow row;

	for(int i = 0; i < ds.size(); i++) {
		ds.getRow(i, row);
		this->add(new CSnmpOid(row));
	}
}

/**
 * Fill a vector with CSnmpOid list.
 */
void BSnmpOidListBase::getSnmpOidVector(vector<CSnmpOid>& oidList) {

	oidList.clear();

	for(size_t i = 0; i < this->size(); i++) {
		CSnmpOid& rSnmpOid = *((CSnmpOid*) this->get(i));
		oidList.push_back(rSnmpOid);
	}
}

/**
 * Create indexes for a SnmpOid.
 * Currently only oid id is used...
 */
void BSnmpOidListBase::addIdx(BSnmpOidBase* pSnmpOid) {
	this->m_idx.setIdx(pSnmpOid->getOidIdStr(), this->size());
}

void BSnmpOidListBase::dumpInfo() const {

	cout << __FILE__ << " pid:" << CSystemTools::doGetPid() << endl;

	for(size_t i = 0; i < this->size(); i++) {
		this->m_oid_list[i]->dumpInfo();
	}

	cout << "Index Count: " << this->m_idx.size() << endl;
}

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */


