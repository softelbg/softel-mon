/*
 * CAvgParamsIfFromCm.cc
 *
 *  Created on: 03.12.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CAvgParamsIfFromCm.h"

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::NET;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace OSS {
namespace CMTS {

CAvgParamsIfFromCm::CAvgParamsIfFromCm(	const string& cmts_id,
										vector<CSnmpOid>& snmp_oids,
										vector<CCmtsIf*>& if_list,
										map<string, int>& map_idx_if,
										vector<CCmtsCm*>& cm_list,
										CCmtsCmIfList& cm_if_list)
	: m_snmp_oid_cmts_if(snmp_oids),
	  m_if_list(if_list),
	  m_map_idx_if(map_idx_if),
	  m_cm_list(cm_list),
	  m_cm_if_list(cm_if_list) {

	this->setCmtsId(cmts_id);

}

CAvgParamsIfFromCm::~CAvgParamsIfFromCm() {
	// TODO Auto-generated destructor stub
}

/**
 * Collect some params Averaging from CM.
 * Averaging is per IF from its CMs.
 */
bool CAvgParamsIfFromCm::collectIfParamsAvgFromCM() {

	unsigned int i;
	unsigned int j;
	unsigned int k;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	// TODO: Consider better algo...(current is nOids x nIFs x nCMs)
	for (i = 0; i < m_snmp_oid_cmts_if.size(); i++) {

		CSnmpOid& rSnmpOid = m_snmp_oid_cmts_if[i];

		// Skip non-empty OIDs
		if (rSnmpOid.isFnAvgFromCM() == false) {
			continue;
		}

		string keyCol = rSnmpOid.getProperty(KEY_PARAM_COL_KEY);
		string mult = rSnmpOid.getProperty(KEY_PARAM_OID_MULT);
		double dMult = atof(mult.c_str());

		// Make Active CM Count
		// TODO: hardcoded, but could be parametrisized
		if (keyCol.compare(KEY_COL_KEY_NUM_ACTIVE_CM) == 0) {
			collectIfNumActiveCM(rSnmpOid);
			continue;
		}

		//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid();
		//cout << " oid:" << rSnmpOid.getSnmpOid() << " colkey:" << keyCol << endl;

		for (j = 0; j < m_if_list.size(); j++) {

			CCmtsIf* pCmtsIf = m_if_list[j];

			// Skip channels different from OID channel
			if (pCmtsIf->isOidSameStreamStrict(rSnmpOid) == false) {
				continue;
			}

			string ifId = pCmtsIf->getIfId();

			double sumVal = 0;
			int numCM = 0;

			//	cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid();
			//	cout << " ifId:" << ifId << endl;

			for (k = 0; k < m_cm_list.size(); k++) {

				CCmtsCm* pCmtsCm = m_cm_list[k];

				// Skip non-active modems
				if (pCmtsCm->isActive() == false) {
					continue;
				}

				CSnmpValue* pCmSnmpValue = NULL;

				// Check for Docs3 availability
				if (this->m_cm_if_list.find(pCmtsCm, pCmtsIf) >= 0) {

					pCmSnmpValue = pCmtsCm->getSnmpValueDocs3(rSnmpOid, pCmtsIf->getIfId());

					if (pCmSnmpValue == NULL) {
						pCmSnmpValue = pCmtsCm->getSnmpValue(keyCol);
					}

				} else {
					// Docs2 cm-if check
					string cmIf = pCmtsCm->getCmIf(rSnmpOid);

					// CM is attached to IF?
					if (ifId.compare(cmIf) == 0) {
						pCmSnmpValue = pCmtsCm->getSnmpValue(keyCol);
					}
				}

				if (pCmSnmpValue != NULL) {

					//		cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid();
					//		cout << " CM:" << pCmtsCm->getCmIp() << endl;

					float currentValue = pCmSnmpValue->getValueFloat();

					if (currentValue > 1000) {

						string snmpValInfo;
						pCmSnmpValue->dumpStrInfo(snmpValInfo);

						cout << __FILE__ << ":" << __FUNCTION__ << ":"
								<< __LINE__ << " pid:" << getpid();
						cout << " cmts: " << this->getCmtsId() << " ifId:"
								<< ifId;
						cout << " CM:" << pCmtsCm->getCmIp();
						cout << " Warning: currentValue:[" << currentValue
								<< "] ";
						cout << "snmpRawValue:[" << pCmSnmpValue->getRawValue()
								<< "] snmp: " << snmpValInfo << endl;

					}

					numCM++;
					sumVal += (currentValue / dMult);

				}

			}

			double resultVal = 0;

			if (numCM > 0) {
				resultVal = sumVal / numCM;
			}

			stringstream ssAvgValue;

			ssAvgValue << resultVal;

			// TODO: Remove the warning check...
			if (resultVal > 1000) {
				cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:"
						<< getpid();
				cout << " ifId:" << ifId << " numCM:" << numCM << " sumVal:"
						<< sumVal << " colkey:"
						<< rSnmpOid.getProperty(KEY_PARAM_COL_KEY);
				cout << " Warning finalVal:" << ssAvgValue.str() << endl;
			}

			//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid();
			//cout << " ifId:" << ifId << " numCM:" << numCM << " sumVal:" << sumVal << " colkey:" << rSnmpOid.getProperty(KEY_PARAM_COL_KEY);
			//cout << " finalVal:" << ssAvgValue.str() << endl;

			pCmtsIf->setSnmpValue(rSnmpOid, ssAvgValue.str());

			// TODO: Remove the warning check...
			string newSnmpValue;
			if (pCmtsIf->getSnmpValue(rSnmpOid.getProperty(KEY_PARAM_OID_ID),
					newSnmpValue)) {
				float fSnmpVal = atof(newSnmpValue.c_str());
				if (fSnmpVal > 80) {
					cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:"
							<< getpid();
					cout << " ifId:" << ifId << " numCM:" << numCM << " sumVal:"
							<< sumVal << " colkey:"
							<< rSnmpOid.getProperty(KEY_PARAM_COL_KEY);
					cout << " Warning fSnmpVal:" << fSnmpVal << endl;

				}
			}

		}

	}

	return true;
}

void CAvgParamsIfFromCm::setCmtsId(const string& cmts_id) {
	this->m_param_set.setParam(KEY_SNMP_OID_CM_CMTS_ID, cmts_id);
}

string CAvgParamsIfFromCm::getCmtsId() {
	return this->m_param_set.getParam(KEY_SNMP_OID_CM_CMTS_ID);
}

CCmtsIf* CAvgParamsIfFromCm::getIfById(const string& key) {

	int idx;
	map<string, int>::iterator it_idx_if;

	it_idx_if = m_map_idx_if.find(key);

	if (it_idx_if != m_map_idx_if.end()) {
		idx = it_idx_if->second;
		return m_if_list[idx];
	} else {
		return NULL;
	}
}

/**
 * Collect count of active CMs for the interface
 */
bool CAvgParamsIfFromCm::collectIfNumActiveCM(CSnmpOid& snmpOid) {

	unsigned int i;

	this->m_cm_if_list.setCmtsId(this->getCmtsId());

	for (i = 0; i < m_cm_list.size(); i++) {

		string cmIfUp;
		string cmIfDwn;

		CCmtsIf* pIfUp = 0;
		CCmtsIf* pIfDwn = 0;

		CCmtsCm* pCmtsCm = m_cm_list[i];

		// Skip non-active modems
		if (pCmtsCm->isActive() == false) {
			continue;
		}

		// Get Up/Dwn Streams for the CM
		cmIfUp = pCmtsCm->getParam(KEY_SNMP_OID_CM_CH_UP);
		cmIfDwn = pCmtsCm->getParam(KEY_SNMP_OID_CM_CH_DWN);

		if (cmIfUp.size()) {
			pIfUp = this->getIfById(cmIfUp);
			if (pIfUp) {
				pIfUp->incActiveCM(snmpOid);
			}
		}

		if (cmIfDwn.size()) {
			pIfDwn = this->getIfById(cmIfDwn);
			if (pIfDwn) {
				pIfDwn->incActiveCM(snmpOid);
			}
		}

		// Multiple IFs list check
		vector<string> if_list;
		this->m_cm_if_list.getIfList(pCmtsCm->getCmMac(), if_list);
		for(int i = 0; i < if_list.size(); i++) {
			if (if_list[i].compare(cmIfUp) != 0 && if_list[i].compare(cmIfDwn) != 0) {
				CCmtsIf*  pIf = this->getIfById(if_list[i]);
				if (pIf) {
					pIf->incActiveCM(snmpOid);
				}
			}
		}

	}

	return true;
}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */




