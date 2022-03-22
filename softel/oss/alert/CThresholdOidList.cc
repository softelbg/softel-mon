/*
 * CThresholdOidList.cc
 *
 *  Created on: 02.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CThresholdOidList.h"

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::CMTS;

namespace SF {
namespace OSS {
namespace ALERT {

CThresholdOidList::CThresholdOidList() {
	// TODO Auto-generated constructor stub

}

CThresholdOidList::CThresholdOidList(const CDataSet& ds) {
	this->init(ds);
}

CThresholdOidList::~CThresholdOidList() {
	clear();
}

void CThresholdOidList::clear() {

	for(int i = 0; i < this->size(); i++) {
		delete this->m_pListThresholds[i];
	}

	this->m_pListThresholds.clear();

	this->m_idx_oid_id.clear();
}

size_t CThresholdOidList::size() const {
	return this->m_pListThresholds.size();
}

/**
 * Init Threshold Oid List from a DataSet.
 * TODO: algo like n2, consider some optimization (still not needed)
 */
void CThresholdOidList::init(const CDataSet& ds) {

	CDataRow row;

	initSnmpOidList();

	for(int i = 0; i < ds.size(); i++) {

		string oidId;

		ds.getRow(i, row);

		if (row.getColumn(KEY_PARAM_OID_ID, oidId) == true) {

			const CSnmpOid* pSnmpOid = this->getSnmpOid(oidId);

			if (pSnmpOid) {

				CThresholdOid* pThreshold = new CThresholdOid(*pSnmpOid);

				pThreshold->init(ds);

				this->add(pThreshold);

			}

		}
	}
}

/**
 * Add new Oid Threshold. If already existed return false and DO NOT Update.
 *
 */
bool CThresholdOidList::add(CThresholdOid* pThreshold) {

	map<string, int>::iterator it;

	it = this->m_idx_oid_id.find(pThreshold->getOidId());

	// Not Found so OK to Add
	if (it == this->m_idx_oid_id.end()) {

		this->m_idx_oid_id[pThreshold->getOidId()] = this->size();
		this->m_idx_col_key[pThreshold->getColKey()] = this->size();

		this->m_pListThresholds.push_back(pThreshold);

	} else {
		return false;
	}

	return true;
}

void CThresholdOidList::initSnmpOidList() {

	CSqlThresholdTools sqlTools;

	sqlTools.selectOidList(this->m_snmp_oid_list);

	// Build oid id index
	for(int i = 0; i < this->m_snmp_oid_list.size(); i++) {
		this->m_idx_oid_id_snmp_oid_list[this->m_snmp_oid_list[i].getOidIdStr()] = i;
	}
}

const CSnmpOid* CThresholdOidList::getSnmpOid(const string& oidId) const {

	map<string, int>::const_iterator it;

	it = this->m_idx_oid_id_snmp_oid_list.find(oidId);

	if (it != this->m_idx_oid_id_snmp_oid_list.end()) {
		return &(this->m_snmp_oid_list[it->second]);
	} else {
		return NULL;
	}
}

/**
 * Get a OidThreshold by Position.
 */
CThresholdOid* CThresholdOidList::get(size_t pos) {

	CThresholdOid* pThreshold;

	if (pos < this->size()) {
		pThreshold = this->m_pListThresholds[pos];
	} else {
		pThreshold = NULL;
	}

	return pThreshold;
}

/**
 * Compare Raw CM (from data row) for the entire oid thresholds list.
 * Every Threshold Result for the respective Oid should be saved with the CM.
 * Finally should be produced a mapped by severity(color) lists with CM->threshold results.
 *
 */
void CThresholdOidList::compareCmRaw(CDataRow& rowCmRaw, map<string, vector<CThresholdResult*> >& listThResults) {

	for(int i = 0; i < size(); i++) {

		CThresholdResult* pThResult = new CThresholdResult();

		string col_key = this->m_pListThresholds[i]->getColKey();

		string value = rowCmRaw.getColumn(col_key);

		this->m_pListThresholds[i]->compare(value, *pThResult);

		pThResult->setCmRaw(rowCmRaw);

		listThResults[pThResult->getParam(PARAM_THRES_SEVERITY)].push_back(pThResult);
	}
}

void CThresholdOidList::dumpInfo() const {

	for(size_t i = 0; i < this->size(); i++) {
		cout << "T[" << i << "] = ";
		this->m_pListThresholds[i]->dumpInfo();
		cout << endl;
	}

}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */



