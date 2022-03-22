/*
 * CCmtsCmAlertList.cc
 *
 *  Created on: 19.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCmtsCmAlertList.h"

#include <softel/oss/cmts/CCmts.h>
#include <softel/oss/cmts/CCmtsCm.h>
#include <softel/core/system/CSystemTools.h>

#include "CSqlCmAlert.h"
#include <softel/db/CDataSet.h>
#include <softel/db/CDataRow.h>
#include <softel/oss/cmts/CSnmpToolsAsynch.h>
#include <softel/oss/alert/COssAlert.h>
#include <softel/oss/alert/CAlertCmDedicated.h>

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::CMTS;
using namespace SF::OSS::ALERT;
using namespace SF::CORE::SYSTEM;
using namespace SF::CORE::UTIL;

namespace SF {
namespace OSS {
namespace EXEC {

CCmtsCmAlertList::CCmtsCmAlertList() {

}

CCmtsCmAlertList::~CCmtsCmAlertList() {

}

/**
 * Initialization of CM List.
 * Should fill initalized CMs into the list.
 *
 * TODO: Consider initialize CM from DataRow...
 */
void CCmtsCmAlertList::initCmList() {

	cout << __FILE__ << ":" << __FUNCTION__ << " pid:"
			<< CSystemTools::doGetPid() << endl;

	CDataSet dsCmList;
	CDataRow row;
	string colName;
	string colValue;

	m_cm_list.clear();

	CSqlCmAlert::getInstance()->sqlGetCmList(dsCmList);

	//dsCmList.dumpInfo();

	dsCmList.resetRowPos();

	while (dsCmList.getNextRow(row)) {

		CCmtsCm* cmtsCM = new CCmtsCm();

		cmtsCM->setParam(row);
		cmtsCM->setCmMacDecimal(row.getColumn("cm_mac"));
		cmtsCM->setParam(KEY_COL_CM_STATUS, STATUS_CM_REGISTRATION_COMPLETE_STR);

		// Validate CM IP
		cmtsCM->setCmValidIp();

		/*
		cmtsCM->setCmtsId(row.getColumn("cmts_id"));
		//cmtsCM->setCmtsIpTools(m_Ip);
		cmtsCM->setRCommunity(row.getColumn("cmts_rcommunity"), row.getColumn("cm_rcommunity"));
		//cmtsCM->setAccountDb(m_dbFactory);
		cmtsCM->setCmMacDecimal(row.getColumn("cm_mac"));
		cmtsCM->setCmSid(row.getColumn("cm_sid"));
		cmtsCM->setParam(KEY_SNMP_OID_CM_IP, row.getColumn("cm_ip"));
		cmtsCM->setParam(KEY_COL_CM_STATUS, STATUS_CM_REGISTRATION_COMPLETE_STR);
*/

		// Set Alert Message CM properties...
		//cmtsCM->addCmAlertMessage("ip:" + row.getColumn("cm_ip"));
		//cmtsCM->addCmAlertMessage(" mac:" + row.getColumn("cm_mac"));
		//cmtsCM->addCmAlertMessage(" rc:" + row.getColumn("cm_rcommunity"));

		m_cm_list.addCm(cmtsCM);

	}

}

/**
 * Do a Monitoring Cycle for the CM List.
 * Should produce for every CM a SnmpValues Set.
 *
 * TODO: Still testing, so consider after success some rework...
 */
void CCmtsCmAlertList::monitorCmList() {

	cout << __FILE__ << ":" << __FUNCTION__ << " pid:" << CSystemTools::doGetPid() << endl;

	CTimeCounter tc;

	// Skip empty CM List
	if (m_cm_list.size() <= 0) {
		return;
	}

	CCmtsCmList listGoodCm;
	CCmtsCmList listAlertCm;
	CCmtsCmList listNonReachableCm;
	CCmtsCmList listNonReachableCmComplementaryCommunity;
	CCmtsCmList listFinalNonReachableCm;

	vector<CSnmpOid> cmOidList;

	CSqlCmAlert::getInstance()->doCmtsCmOidList(m_cm_list.getCm(0)->getCmtsId(), cmOidList);

	// First Full CM Sweep
	doAsynchMonitorCmList(m_cm_list, listGoodCm, listAlertCm, listNonReachableCm, cmOidList);

	// Second NonReachable CM Sweep
	if (listNonReachableCm.size() > 0) {
		doAsynchMonitorCmList(listNonReachableCm, listGoodCm, listAlertCm, listNonReachableCmComplementaryCommunity, cmOidList);
	}

	// Third NonReachable CM Sweep using complementary Community
	// TODO: Consider get distinct communities and sweep all of them
	if (listNonReachableCmComplementaryCommunity.size() > 0) {

		for(int i = 0; i < listNonReachableCmComplementaryCommunity.size(); i++) {
			listNonReachableCmComplementaryCommunity.getCm(i)->setParam(KEY_COL_CM_RCOMM, "modems");
		}

		doAsynchMonitorCmList(listNonReachableCmComplementaryCommunity, listGoodCm, listAlertCm, listFinalNonReachableCm, cmOidList);
	}

	// Do something with final sweep non reachable...
	if (listFinalNonReachableCm.size() > 0) {

		// Copy final non reachable to Alert List
		for(int i = 0; i < listFinalNonReachableCm.size(); i++) {

			listFinalNonReachableCm.getCm(i)->addEventMessage("Non Reachable");

			listAlertCm.addCm(listFinalNonReachableCm.getCm(i));

			//listFinalNonReachableCm.getCm(i)->dumpCmInfo();

		}

	}

	// Fire Alert for every alerted CM in alert list.
	for(int i = 0; i < listAlertCm.size(); i++) {

		CAlertCmDedicated cmAlert(listAlertCm.getCm(i));

		cmAlert.saveAlert();

	}

	CDataSet dsCmAlert;

	CSqlCmAlert::getInstance()->sqlGetCmAlertList(dsCmAlert);

	// Close Alert for every good CM if there is an open one.
	for(int i = 0; i < listGoodCm.size(); i++) {

		CCmtsCm* pCm = listGoodCm.getCm(i);

		CDataRow row;

		int pos = dsCmAlert.searchIndexedColumn(KEY_COL_CM_MAC, pCm->getCmMac());

		if (pos >= 0) {

			dsCmAlert.getRow(pos, row);

			CAlertCmDedicated cmAlert(pCm);

			cmAlert.initAlert(row);

			cmAlert.doCloseAlert();

		}

	}

	cout << "  Elapsed:" << tc.getElapsed() << " s    " << tc.getCTimeNow();
}

/**
 * Do single Asynch monitoring cycle for a CM List.
 * Put Alerted CMs into Alert CM List.
 * Put Not Reachable CMs into NonReachable CM List.
 *
 */
void CCmtsCmAlertList::doAsynchMonitorCmList(	CCmtsCmList& listCm,
												CCmtsCmList& listGoodCm,
												CCmtsCmList& listAlertCm,
												CCmtsCmList& listNonReachable,
												vector<CSnmpOid>& cmOidList) {

	CSnmpToolsAsynch snmpAsynch;

	snmpAsynch.snmpMultipleGetAsynch(listCm.getCmList(), cmOidList);

	for (int i = 0; i < listCm.size(); i++) {

		CSnmpValue* pSnmpValue = listCm.getCm(i)->getSnmpValue(KEY_COL_KEY_DWN_SNR);

		// Do a Check for good/bad CM - check DWN SNR now...
		if (listCm.getCm(i)->isDwnSnrValid() == false) {

			if (pSnmpValue) {

				string keyCol = pSnmpValue->getSnmpOid().getProperty(KEY_PARAM_COL_KEY);

				listCm.getCm(i)->addEventMessage(keyCol + " = " + pSnmpValue->getValue());

				listAlertCm.addCm(listCm.getCm(i));

				//pSnmpValue->dumpInfo();

			} else {

				listNonReachable.addCm(listCm.getCm(i));

			}

			cout << "ALERT CM ip:" << listCm.getCm(i)->getCmIp();
			cout << " mac:"
					<< listCm.getCm(i)->getParam(KEY_SNMP_OID_CM_MAC_DEC)
					<< endl;

		} else {

			listGoodCm.addCm(listCm.getCm(i));

		}

	}

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

