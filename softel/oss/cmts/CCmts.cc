/**
 * CMTS Representation
 *
 *	@author sgeorgiev
 *
 *	www.softel.bg
 *
 *	Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CCmts.h"

#include <softel/core/error/CErrorSql.h>
#include <softel/core/error/CErrorCommon.h>

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::NET;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace OSS {
namespace CMTS {

CCmts::CCmts(const int cmts_id, const string& cmts_ip,
		const string& cmts_rcommunity, const string& cm_rcommunity,
		const string& cmts_desc, const int cmts_docsis_ver) :
	m_dbFactory(NULL), m_Ip(NULL) {

	setCmtsId(cmts_id);
	setCmtsIp(cmts_ip);
	setCmtsRComunity(cmts_rcommunity);
	setCmRComunity(cm_rcommunity);
	setCmtsVer(cmts_docsis_ver);
	setCmtsDesc(cmts_desc);

	setPeriodSleep(600);

}

CCmts::CCmts(const string& cmts_id, const string& cmts_ip,
		const string& cmts_rcommunity, const string& cm_rcommunity,
		const string& cmts_desc, const int cmts_docsis_ver) :
	m_dbFactory(NULL), m_Ip(NULL) {

	setCmtsId(cmts_id);
	setCmtsIp(cmts_ip);
	setCmtsRComunity(cmts_rcommunity);
	setCmRComunity(cm_rcommunity);
	setCmtsVer(cmts_docsis_ver);
	setCmtsDesc(cmts_desc);

	setPeriodSleep(600);

}

CCmts::~CCmts() {
	if (m_Ip != NULL) {
		delete m_Ip;
	}
}

/**
 * Raise Stop Collecting Flag
 */
void CCmts::stopCollecting() {
	m_stopCollecting = true;
}

/**
 * Check if the Stop Collecting Flag is raised.
 */
bool CCmts::isStoppedCollecting() {
	return m_stopCollecting;
}

/**
 * Start a Single Full CMTS Collecting Cycle.
 * Collect CM params, collect IF params...
 */
void CCmts::startCmtsCollectingSingle() {

	clearCmList();
	clearIfList();

	collectCmParams();

	collectIfParams();

}

/**
 * Start Continuous Full CMTS Collecting Cycle.
 * Should never exit from this state (perhaps some exit flag should implement...)
 * Collect CM params, collect IF params...
 * Make FORK for the single collecting cycle.
 */
void CCmts::startCmtsCollecting() {

	CTimeCounter tc;

	m_stopCollecting = false;

	int childStatus = 0;

	while(true) {

		tc.reset();

		LOGD << "Collection Started for cmts: " << m_cmts_id << " ip:" << m_cmts_ip;
		LOGD << " desc:" << m_cmts_desc << "\t" << tc.getCTimeNow() << endl;

		int pidMain = getpid();

		int pidChild = CSystemTools::doFork();

		if (pidChild == 0) {

			try {

			startCmtsCollectingSingle();

			} catch (CErrorSql& error) {
				LOG(LOG_LEVEL_ERROR, error.getMessage());
			} catch (CErrorCommon& error) {
				LOG(LOG_LEVEL_ERROR, error.getMessage());
			}
	
			exit(0);	
		}

		int pidWaitedChild = CSystemTools::doWait(&childStatus);

		if (pidWaitedChild != pidChild) {
			LOGD << __FUNCTION__ << ": Error: pid: " << pidMain << " pidWaitedChild: " << pidWaitedChild << " pidChild: " << pidChild << endl;
		}

		LOGD << "Collection Finished for cmts: " << m_cmts_id << " ip:" << m_cmts_ip;
		LOGD << " desc:" << m_cmts_desc << " with status:" << childStatus << "\t" << tc.getElapsedFormated();
		LOGD << "\t" << tc.getCTimeNow() << endl;

		int periodWait = m_periodSleep - tc.getElapsed();

		if (periodWait > 0) {
			CSystemTools::doSleep(periodWait);
		}

		if (isStoppedCollecting()) {
			break;
		}

	}

}

/**
 * Make IFs List
 */
bool CCmts::refreshIfListBulk() {


	string strSnmp;
	string strIfId;
	string strIfName;

	vector < string > snmpWalkOID;
	vector < string > snmpWalkResult;

	snmpWalkOID.clear();
	snmpWalkResult.clear();

	clearIfList();

	initIfParamsMap();

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	// TODO: Consider rework....
	if (m_Ip->SnmpWalkBulk(	SNMP_OID_IFNAME,
						m_cmts_rcommunity,
						&snmpWalkOID,
						&snmpWalkResult)) {

		for (unsigned int i = 0; i < snmpWalkResult.size(); i++) {

			strIfId = snmpWalkOID[i];
			strIfName = snmpWalkResult[i];

			// Add new CMTS IF to IF List
			addIfById(strIfId);
			setIfParamById(strIfId, KEY_SNMP_OID_IF_NAME, strIfName);
		}

	}

	return true;
}

bool CCmts::updateIfParamBulk() {

	map<string, string>::iterator it;

	unsigned int i;

	vector < string > snmpWalkOID;
	vector < string > snmpWalkResult;

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	for (it = m_map_oid_if_params.begin(); it != m_map_oid_if_params.end(); it++) {

		LOGD << __FILE__ << ":" << __FUNCTION__ << ": SnmpWalk " << it->second << endl;

		snmpWalkOID.clear();
		snmpWalkResult.clear();

		if (m_Ip->SnmpWalkBulk(it->second.c_str(), m_cmts_rcommunity.c_str(),
				&snmpWalkOID, &snmpWalkResult)) {

			//LOGD << __FILE__ << ":" << __FUNCTION__ << ": SnmpWalk " << it->second << " RCVsize=" << snmpWalkResult.size() << endl;

			for (i = 0; i < snmpWalkResult.size(); i++) {
				setIfParamById(snmpWalkOID[i], it->first, snmpWalkResult[i]);
			}

		}

	}

	return true;
}

/**
 * Collect count of active CMs for the interface
 */
bool CCmts::collectIfNumActiveCM(CSnmpOid& snmpOid) {

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
			pIfUp = getIfById(cmIfUp);
			if (pIfUp) {
				pIfUp->incActiveCM(snmpOid);
			}
		}

		if (cmIfDwn.size()) {
			pIfDwn = getIfById(cmIfDwn);
			if (pIfDwn) {
				pIfDwn->incActiveCM(snmpOid);
			}
		}

		// Multiple IFs list check
		vector<string> if_list;
		this->m_cm_if_list.getIfList(pCmtsCm->getCmMac(), if_list);
		for(int i = 0; i < if_list.size(); i++) {
			if (if_list[i].compare(cmIfUp) != 0 && if_list[i].compare(cmIfDwn) != 0) {
				CCmtsIf*  pIf = getIfById(if_list[i]);
				if (pIf) {
					pIf->incActiveCM(snmpOid);
				}
			}
		}

	}

	return true;
}

/**
 * Collect some params Averaging from CM.
 * Averaging is per IF from its CMs.
 */
bool CCmts::collectIfParamsAvgFromCM() {

	//LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	CAvgParamsIfFromCm avgParams(	this->getCmtsId(),
									m_snmp_oid_cmts_if,
									m_if_list,
									m_map_idx_if,
									m_cm_list,
									m_cm_if_list);

	avgParams.collectIfParamsAvgFromCM();

	return true;
}

/**
 * Collect Snmp for IF from CMTS (for ex. SNR, RxPwr, TO, MR...).
 * Store as IF Param (not in DB). IF list should be created already...
 */
bool CCmts::collectSnmpIfBulkParams() {

	unsigned int i;
	unsigned int j;

	vector < string > listIfId;
	vector < string > listSnmpValues;

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	initCmtsIfTable();

	for (i = 0; i < m_snmp_oid_cmts_if.size(); i++) {

		CSnmpOid& rSnmpOid = m_snmp_oid_cmts_if[i];

		listIfId.clear();
		listSnmpValues.clear();

		// Skip empty OIDs
		if (rSnmpOid.getSnmpOid().size() == 0) {
			continue;
		}

		LOGD << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid()
				<< " cmts:" << m_cmts_id << ":" << m_cmts_ip;
		LOGD << " SnmpWalk " << rSnmpOid.getSnmpOid();

		if (m_Ip->SnmpWalkBulk(rSnmpOid.getSnmpOid(), m_cmts_rcommunity,
				&listIfId, &listSnmpValues)) {

			// Add the SNMP Value to the IF in the list
			for (j = 0; j < listIfId.size(); j++) {

				CCmtsIf* cmtsIf = getIfById(listIfId[j]);

				if (cmtsIf != NULL) {

					// Set explicitly up/dwn stream type for such IFs
					string keyCol;
					rSnmpOid.getProperty(KEY_PARAM_COL_KEY, keyCol);
					if (keyCol.compare(KEY_COL_DWN_TXPWR) == 0) {
						cmtsIf->setTypeDownstream();
					}

					cmtsIf->setSnmpValue(rSnmpOid, listSnmpValues[j]);

				} else {
					LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__
							<< ":" << listIfId[j] << " Not Found!" << endl;
				}

			}

		} else {
			LOGD << " Error SnmpWalkBulk";
		}

		LOGD << " size:" << listSnmpValues.size() << endl;

	}

	return true;
}


/**
 * Refresh interface on CMTS and add if missing
 */
bool CCmts::refreshInterfaces() {

	char snmpResult[256] = { 0 };

	string strSnmp;
	string strIfId;
	string strIfName;

	vector < string > snmpWalkOID;
	vector < string > snmpWalkResult;

	snmpWalkOID.clear();
	snmpWalkResult.clear();

	clearIfList();

	//LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	// TODO: Consider rework....
	if (m_Ip->SnmpWalkBulk(	SNMP_OID_IFNAME,
						m_cmts_rcommunity.c_str(),
						&snmpWalkOID,
						&snmpWalkResult)) {

		m_Ip->InitSnmpSession(m_cmts_rcommunity.c_str());

		for (unsigned int i = 0; i < snmpWalkResult.size(); i++) {

			strIfId = snmpWalkOID[i];
			strIfName = snmpWalkResult[i];

			// Add new CMTS IF to IF List
			addIfById(strIfId);
			setIfParamById(strIfId, KEY_SNMP_OID_IF_NAME, strIfName);

			//Check interface already in oss_cmts_inerface
			if (m_tools_sql.checkInterfaceExists(pDb, strIfId) == false) {

				m_tools_sql.insertInterface(pDb, strIfId, strIfName);

				// Get and set type of new interface
				strSnmp = SNMP_OID_IFTYPE".";
				strSnmp += strIfId;

				m_Ip->SnmpGet(	strSnmp,
								snmpResult);

				m_tools_sql.updateInterfaceType(pDb, strIfId, snmpResult);

				setIfParamById(strIfId, KEY_SNMP_OID_IF_TYPE, snmpResult);
			}
		}

		m_Ip->CloseSnmpSession();

	}

	delete pDb;

	return true;
}

/**
 * Collect full set of params for all CMs for this CMTS.
 * Should be used as single point of CMTS CM list creator.
 *
 */
bool CCmts::collectCmParams() {

	CTimeCounter tc;
	CTimeCounter tc2;

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "\tpid:" << getpid();
	LOGD << "\t" << tc.getCTimeNow() << endl;

	// Read Snmp List for CM and IFs.
	doCmtsCmSnmpList();
	doCmtsCmSnmpDocs3List();
	doCmSnmpList();
	doCmtsIfSnmpList();
	doSnmpList();

	selectCmRCommList();

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid();
	LOGD << "\tcmts:" << m_cmts_id << ":ip:" << m_cmts_ip << "\t" << tc2.getElapsedFormated() << endl;
	tc2.reset();

	// Create initial CM map with some of the params
	refreshCmListBulk();

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid();
	LOGD << "\tcmts:" << m_cmts_id << ":ip:" << m_cmts_ip << "\t" << tc2.getElapsedFormated() << endl;
	tc2.reset();

	updateCmParamBulk();

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid();
	LOGD << "\tcmts:" << m_cmts_id << ":ip:" << m_cmts_ip << "\t" << tc2.getElapsedFormated() << endl;
	tc2.reset();

	collectSnmpCmBulkParams();
	collectSnmpCmBulkParamsDocs3();	
	collectSnmpCmBulkParamsAux();

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid();
	LOGD << "\tcmts:" << m_cmts_id << ":ip:" << m_cmts_ip << "\t" << tc2.getElapsedFormated() << endl;
	tc2.reset();

	collectSnmpFromCmAsynch();

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid();
	LOGD << "\tcmts:" << m_cmts_id << ":ip:" << m_cmts_ip << "\t" << tc2.getElapsedFormated() << endl;
	tc2.reset();
		
	// Finished with snmp collection, now make some snmp values process...
	try {
	
	postProcessCmList();
	
	doDeleteOldCmToday();
	this->doUpdateTodayCmMinDate();

	}
  	catch (CErrorSql& error) {
		LOG(LOG_LEVEL_ERROR, error.getMessage());
	}
	catch (CErrorCommon& error) {
		LOG(LOG_LEVEL_ERROR, error.getMessage());
	}

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid();
	LOGD << "\tcmts:" << m_cmts_id << ":ip:" << m_cmts_ip << "\t" << tc2.getElapsedFormated() << "\tSum of " << tc.getElapsedFormated() << endl;

	return true;
}

/**
 * Collect full set of params for all IFs for this CMTS.
 * Should be used as single point of CMTS IF list creator.
 *
 */
bool CCmts::collectIfParams() {

	CTimeCounter tc;
	CTimeCounter tc2;

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "\tpid:"
			<< getpid();
	LOGD << "\t" << tc.getCTimeNow() << endl;

	refreshIfListBulk();

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:"
			<< getpid();
	LOGD << "\tcmts:" << m_cmts_id << ":ip:" << m_cmts_ip << "\t"
			<< tc2.getElapsedFormated() << endl;
	tc2.reset();

	updateIfParamBulk();

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:"
			<< getpid();
	LOGD << "\tcmts:" << m_cmts_id << ":ip:" << m_cmts_ip << "\t"
			<< tc2.getElapsedFormated() << endl;
	tc2.reset();

	collectSnmpIfBulkParams();

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:"
			<< getpid();
	LOGD << "\tcmts:" << m_cmts_id << ":ip:" << m_cmts_ip << "\t"
			<< tc2.getElapsedFormated() << endl;
	tc2.reset();

	collectIfParamsAvgFromCM();

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:"
			<< getpid();
	LOGD << "\tcmts:" << m_cmts_id << ":ip:" << m_cmts_ip << "\t"
			<< tc2.getElapsedFormated() << endl;
	tc2.reset();

	// Finished with snmp collection, now make some snmp values process...
	try {

		postProcessIfList();

		doDeleteOldIfToday();
		this->doUpdateTodayIfMinDate();

	} catch (CErrorSql& error) {
		LOG(LOG_LEVEL_ERROR, error.getMessage());
	} catch (CErrorCommon& error) {
		LOG(LOG_LEVEL_ERROR, error.getMessage());
	}

	//dumpIfList();

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:"
			<< getpid();
	LOGD << "\tcmts:" << m_cmts_id << ":ip:" << m_cmts_ip << "\t"
			<< tc2.getElapsedFormated() << "\tSum of "
			<< tc.getElapsedFormated() << endl;

	return true;
}

/**
 * Get ROW with CM Error Counters - read previously from DB oss_cm_errors
 */
void CCmts::setCmErrorsRow(CCmtsCm* pCm) {

	CDataSet& ds = m_ds_cm_errors;
	CDataRow row;
	int rowPos = -1;

	rowPos = ds.searchIndexedColumn(KEY_COL_CM_MAC, pCm->getCmMac());

	if (rowPos >= 0) {

		ds.getRow(rowPos, row);

		//row.dumpInfo();

		pCm->setErrorCountersRow(row);

	} else {

		//LOGD << "Not Found:" << pCm->getCmMac() << endl;

	}

}

string CCmts::getCmtsId() {
	return this->m_param_set.getParam(KEY_COL_CMTS_ID);
}

/**
 * Select Full CM RComm List.
 * When there are wrong RComm configured CMs should be queried by other from the list.
 *
 */
bool CCmts::selectCmRCommList() {

	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	m_tools_sql.selectCmRCommList(pDb, m_list_cm_rcomm);

	delete pDb;

	return true;
}

/**
 * Get ROW with IF Error Counters - read previously from DB oss_if_errors
 */
void CCmts::setIfErrorsRow(CCmtsIf* pIf) {

	CDataSet& ds = m_ds_if_errors;
	CDataRow row;
	int rowPos = -1;

	rowPos = ds.searchIndexedColumn(KEY_COL_IF_ID, pIf->getParam(KEY_SNMP_OID_IF_ID));

	if (rowPos >= 0) {

		ds.getRow(rowPos, row);

		//row.dumpInfo();

		pIf->setErrorCountersRow(row);

	} else {

		//LOGD << "Not Found:" << pIf->getParam(KEY_SNMP_OID_IF_ID) << endl;

	}

}

/**
 * Post Process all CMs after all SNMP values are collected.
 */
bool CCmts::postProcessCmList() {

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << " CMcount=" << m_cm_list.size() << endl;

	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	doCmErrorsList();

	CTimeCounter tc1;
	CTimeCounter tc2;

	// Delete previous Docs3 CM IFs in Docs3 CM List
	m_tools_sql.deleteCmListDocs3(pDb);
	// Delete previous CM CPE List
	m_tools_sql.deleteCmCpeList(pDb);

	// Use CM vector list
	for(unsigned int i = 0, k = 0; i < m_cm_list.size(); i++, k++) {

		//m_cm_list[i]->processAllSnmpValues();

		m_cm_list[i]->setSnmpOidList(&m_snmp_oid);
		
		setCmErrorsRow(m_cm_list[i]);

		m_cm_list[i]->save(pDb);
		//m_cm_list[i]->dumpCmInfo();

		if (k >= 1000) {
			LOGD << __FUNCTION__ << ":pid:" << getpid() << " Processed " << i << " from " << m_cm_list.size() << " CMs" << " Elapsed " << tc1.getElapsed() << endl;
			k = 0;
			tc1.reset();
		}
	}

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << " Elapsed " << tc2.getElapsed() << endl;

	delete pDb;

	return true;
}

/**
 * Post Process all IFs after all SNMP values are collected.
 */
bool CCmts::postProcessIfList() {

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << " IFcount=" << m_if_list.size() << endl;

	doIfErrorsList();

	initModProfiles();

	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	for(unsigned int i = 0; i < m_if_list.size(); i++) {

		m_if_list[i]->setSnmpOidList(&m_snmp_oid);
		m_if_list[i]->setModProfilesList(&m_modprofiles);

		setIfErrorsRow(m_if_list[i]);

		m_if_list[i]->save(pDb);

		//m_if_list[i]->dumpInfo();
	}

	delete pDb;

	return true;
}

/**
 * Initalize Modulation Profiles for the CMTS.
 */
void CCmts::initModProfiles() {

	m_modprofiles.setIp(m_Ip);

	m_modprofiles.initModProfilesList();

	//m_modprofiles.dumpInfo();
}

/**
 * Collect all CM Snmp direct from CM.
 *
 * Snmp is asynchronous (net-snmp launch pthreads to achieve this,
 * so not needed our thread implementation for this)
 *
 * SNMP OIDs which should be queried from CM are distinguished
 * in table oss_cpe_oid_types with column "from_cm != 0".
 * List of CMs is already filled and should query only Active CMs.
 */
bool CCmts::collectSnmpFromCmAsynch() {

	//LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	unsigned int maxAsynchCM = 7000;
	unsigned int maxCmtsAsynchCm = 5000;

	CSnmpToolsAsynch snmpAsynch;

	CTimeCounter tcLocked;

	// Wait while unlocked IPC-lock
	while (m_pSharedCounter->isLocked()) {
		//LOGD << __FUNCTION__ << ":pid:" << getpid() << " Waiting for IPC-lock..." << endl;

		// Do NOT Wait if Sum of all Asynch CMs are bellow maxAsynchCM
		if (m_pSharedCounter->getCounter(0) + getCmCount() < maxAsynchCM) {
			break;
		} 		 

		// Do NOT Wait too long if a small CMTS we have...
		if (tcLocked.getElapsed() > 30 && getCmCount() < 1000) {
			break;
		}

		// Do NOT Wait too long if too big CMTS we have...
		if (getCmCount() > maxAsynchCM) {
			//break;
		}

		CSystemTools::doSleep(1);
	}

	LOGD << __FUNCTION__ << ":pid:" << getpid() << "\tcmts:" << m_cmts_id << ":ip:" << m_cmts_ip;
	LOGD << "\t" << tcLocked.getElapsedFormated() << "waiting for IPC-lock" << endl;

	m_pSharedCounter->incCounter(0, getCmCount());
	m_pSharedCounter->setLock(true);

	CTimeCounter tc;

	LOGD << __FUNCTION__ << ":Locked by pid:" << getpid();
	LOGD << ": cmtsId:" << m_cmts_id;
	LOGD << " :numCM:" << getCmCount() << " asynchCM:" << m_pSharedCounter->getCounter(0) << "   " << tc.getCTimeStart();

	//snmpAsynch.snmpMultipleGetAsynch(m_cm_list, m_snmp_oid_cm);

	
	snmpAsynch.snmpMultipleGetAsynch(
										 m_cm_list,
										 m_snmp_oid_cm,
										 maxCmtsAsynchCm);
	 
	// Check for non processed (Dwn SNR not valid) and snmp query again...
	vector<CCmtsCm*> listCmNonProcessed;

	// Switch other RComm for the rest of non processed modems...
	string thisCmRComm;
	this->m_param_set.getParam(KEY_COL_CM_RCOMM, thisCmRComm);

	for(int j = 0; j < this->m_list_cm_rcomm.size(); j++) {

		CDataRow row;

		m_list_cm_rcomm.getRow(j, row);
		string cmRComm;
		row.getColumn(KEY_COL_CM_RCOMM, cmRComm);

		//if (cmRComm.compare(thisCmRComm) != 0) {

			listCmNonProcessed.clear();

			for(size_t i = 0; i < m_cm_list.size(); i++) {

				if (m_cm_list[i]->isDwnSnrValid() == false) {

					m_cm_list[i]->setParam(KEY_COL_CM_RCOMM, cmRComm);

					listCmNonProcessed.push_back(m_cm_list[i]);
				}

			}
			// NonProcessed2 list process
			snmpAsynch.snmpMultipleGetAsynch(
												listCmNonProcessed,
												m_snmp_oid_cm,
												maxCmtsAsynchCm);
		//}
	}

	int NPF = 0;
	for (size_t i = 0; i < getCmCount(); i++) {
		if (m_cm_list[i]->isDwnSnrValid() == false) {
			NPF++;
		}
	}

	LOGD << __FUNCTION__ << ":Unlocked by pid:" << getpid();
	LOGD << ": cmtsId:" << m_cmts_id << " :numCM:" << getCmCount();
	LOGD << " NP1:" << listCmNonProcessed.size();
	LOGD << " NPF:" << NPF;
	LOGD << "  Elapsed:" << tc.getElapsed() << " s    " << tc.getCTimeNow();

	m_pSharedCounter->decCounter(0, getCmCount());
	m_pSharedCounter->setLock(false);

	return true;
}


/**
 * Create new CM List from CMTS.
 * Fill MAP with all CMs.
 */
bool CCmts::refreshCmListBulk() {

	vector <string> snmpWalkOID;
	vector <string> snmpWalkResult;

	//LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	initCmTable();
	
	initCmParamsMap();
	
	snmpWalkOID.clear();
	snmpWalkResult.clear();
	
	//clearMapCm();
	clearCmList();

	// Get MAC/SID list and create CM list
	if (m_Ip->SnmpWalkBulk(	SNMP_OID_CM_LIST,
							m_cmts_rcommunity.c_str(),
							&snmpWalkOID,
							&snmpWalkResult)) {

		//LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

		for (unsigned int i = 0; i < snmpWalkOID.size(); i++) {

			CCmtsCm* cmtsCM = new CCmtsCm();

			cmtsCM->setCmtsId(m_cmts_id);
			cmtsCM->setCmtsIpTools(m_Ip);
			cmtsCM->setRCommunity(m_cmts_rcommunity, m_cm_rcommunity);
			cmtsCM->setAccountDb(m_dbFactory);

			string& cmMac = snmpWalkResult[i];
			string& cmSid = snmpWalkOID[i];

			cmtsCM->setCmMac(cmMac);
			cmtsCM->setCmSid(cmSid);

			addCmBySid(cmSid, cmtsCM);

		}

	}

	return true;
}

void CCmts::initCmParamsMap() {

	m_map_oid_cm_params.clear();

	m_map_oid_cm_params[KEY_COL_CM_IP] = SNMP_OID_CM_IP;
	m_map_oid_cm_params[KEY_COL_CM_STATUS] = SNMP_OID_CM_STATUS;
	m_map_oid_cm_params[KEY_SNMP_OID_CM_CH_DWN] = SNMP_OID_CM_CH_DWN;
	m_map_oid_cm_params[KEY_SNMP_OID_CM_CH_UP] = SNMP_OID_CM_CH_UP;

}

void CCmts::initIfParamsMap() {

	m_map_oid_if_params.clear();

	//m_map_oid_if_params[KEY_SNMP_OID_IF_NAME] = SNMP_OID_IFNAME;
	m_map_oid_if_params[KEY_SNMP_OID_IF_TYPE] = SNMP_OID_IFTYPE;
	m_map_oid_if_params[KEY_SNMP_OID_IF_DESC] = SNMP_OID_IFDESC;
	//m_map_oid_if_params[KEY_SNMP_OID_IF_ALIAS] = SNMP_OID_IF_ALIAS;

}

/**
 * Update main CM list Params from CMTS.
 *
 */
bool CCmts::updateCmParamBulk() {

	map<string, string>::iterator it;

	unsigned int i;

	vector <string> snmpWalkOID;
	vector <string> snmpWalkResult;

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << " param.size:" << m_map_oid_cm_params.size() << endl;

	for(it = m_map_oid_cm_params.begin(); it != m_map_oid_cm_params.end(); it++) {

		CTimeCounter tc;

		//LOGD << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid() << " SnmpWalk " << it->second << " " << tc.getCTimeStart();

		snmpWalkOID.clear();
		snmpWalkResult.clear();

		if (m_Ip->SnmpWalkBulk(		it->second.c_str(),
									m_cmts_rcommunity.c_str(),
									&snmpWalkOID,
									&snmpWalkResult)) {

			//LOGD << __FILE__ << ":" << __FUNCTION__ << ": SnmpWalk " << it->second << " RCVsize=" << snmpWalkResult.size() << endl;

			for(i = 0; i < snmpWalkResult.size(); i++) {
				//setMapCmParam(snmpWalkOID[i], it->first, snmpWalkResult[i]);
				setCmParamBySid(snmpWalkOID[i], it->first, snmpWalkResult[i]);
			}

		} else {
			LOGD << __FUNCTION__ << ":pid:" << getpid() << " Error SnmpWalk " << it->second << endl;
		}

		LOGD << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid() << " cmts:" << m_cmts_id << ":" << m_cmts_ip;
		LOGD << " SnmpWalk " << it->second << " size:" << snmpWalkResult.size() << " Elapsed " << tc.getElapsed() << " s " << endl;

	}


	return true;
}

/**
 * Collect Snmp for CM from CMTS (for ex. SNR, RxPwr, TO, MR...).
 * Store as CM Param (not in DB). CM list should be created already...
 * TODO: Consider make it asynch...
 */
bool CCmts::collectSnmpCmBulkParams() {

	unsigned int i;
	unsigned int j;

	vector <string> listSid;
	vector <string> listSnmpValues;

	LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	for (i = 0; i < m_snmp_oid_cmts.size(); i++) {

		CTimeCounter tc;

		CSnmpOid& rSnmpOid = m_snmp_oid_cmts[i];

		listSid.clear();
		listSnmpValues.clear();

		if (m_Ip->SnmpWalkBulk(	rSnmpOid.getSnmpOid(),
								m_cmts_rcommunity,
								&listSid, &listSnmpValues)) {

		
			// Save Snmp Values as CM Param
			for(j = 0; j < listSid.size(); j++) {
				CCmtsCm* cm;

				// Use CM vector list (indexed by maps)
				cm = getCmBySid(listSid[j]);
				if (cm != NULL) {
					cm->setParam(rSnmpOid, listSnmpValues[j]);
				}
			}


		}

		LOGD << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid() << " cmts:" << m_cmts_id << ":" << m_cmts_ip;
                LOGD << " SnmpWalk " << rSnmpOid.getSnmpOid() << " size:" << listSnmpValues.size() << " Elapsed " << tc.getElapsed() << " s " << endl;
	}

	return true;
}

/**
 * Collect DOCSIS 3.0 Snmp for CM from CMTS (for ex. SNR, RxPwr, TO, MR...).
 * Store as CM Param (not in DB). CM list should be created already...
 * TODO: Consider make it asynch...
 */
bool CCmts::collectSnmpCmBulkParamsDocs3() {

	unsigned int i;

	CDataSet result;
	CDataRow row;

	//LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	for (i = 0; i < m_snmp_oid_cmts_docs3.size(); i++) {

		CTimeCounter tc;

		CSnmpOid& rSnmpOid = m_snmp_oid_cmts_docs3[i];

		result.clear();

		if (m_Ip->SnmpWalkBulkDocs3(
								rSnmpOid.getSnmpOid(),
								m_cmts_rcommunity,
								result)) {

			//LOGD << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid();
			//LOGD << " oid:" << rSnmpOid.getSnmpOid() << " result.size:" << result.size() << endl;
			//result.dumpInfo();

			result.resetRowPos();

			CCmtsCm* cm;
			
			while(result.getNextRow(row)) {

				string sid;
				string ifid;
				string snmpVal;

				row.getColumn(KEY_SID, sid);
				row.getColumn(KEY_IF_ID, ifid);
				row.getColumn(KEY_VALUE, snmpVal);

//				LOGD << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid();
//				LOGD << " sid:" << sid << " ifid:" << ifid << " snmpVal:" << snmpVal << endl;

				cm = getCmBySid(sid);

				if (cm != NULL) {
	
					cm->setDocs3SnmpValue(rSnmpOid, ifid, snmpVal);

					this->m_cm_if_list.add(*cm, ifid); // Add to CM<->IF list

				//	cm->dumpCmInfo();

				} else {
					LOGD << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid();
					LOGD << "SID: " << sid << " Not Found" << endl;
				}

			}

		}

		LOGD << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid() << " cmts:" << m_cmts_id << ":" << m_cmts_ip;
		LOGD << " Snmp: " << rSnmpOid.getSnmpOid() << " size:" << result.size() << " Elapsed " << tc.getElapsed() << " s " << endl;
	}

	return true;
}

/**
 * Collect some AUX CM Params.
 * Currently the CPE List is collected in this way
 * hardcoded SNMP Params, not in DB...
 */
bool CCmts::collectSnmpCmBulkParamsAux() {

	// TODO: Consider make these params better initialized from DB, not hardcoded here...

	CDataSet result;
	CDataSet resultSid;
	CDataRow row;

	m_Ip->SnmpWalkBulkIndexedByMAC(
										SNMP_OID_CMTS_MAC2SID,
										m_cmts_rcommunity,
										resultSid);

	resultSid.buildColumnIndex(KEY_MAC_DEC);

	//resultSid.dumpInfo();

	// Collect CPE List
	if (m_Ip->SnmpWalkBulkIndexedByMAC(
									SNMP_OID_CMTS_CPE_IP,
									m_cmts_rcommunity,
									result)) {
		result.resetRowPos();

		CCmtsCm* cm;

		while (result.getNextRow(row)) {

			string hexMac;
			string decMac;
			string snmpVal;
			string sid;

			row.getColumn(KEY_MAC_HEX, hexMac);
			row.getColumn(KEY_MAC_DEC, decMac);
			row.getColumn(KEY_VALUE, snmpVal);

			int sidPos = resultSid.searchIndexedColumn(KEY_MAC_DEC, decMac);

			if (sidPos < 0) {
				continue;
			}

			CDataRow rowSid;

			resultSid.getRow(sidPos, rowSid);

			rowSid.getColumn(KEY_VALUE, sid);

			//LOGD << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid();
			//LOGD << " SID:" << sid;
			//LOGD << " mac:" << hexMac << " " << decMac << " snmpVal:" << snmpVal << endl;

			cm = getCmBySid(sid);

			if (cm != NULL) {

				CCpe cpe(snmpVal, hexMac);

				cm->addCpe(cpe);


			} else {
				LOGD << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid();
				LOGD << " SID:" << sid;
				LOGD << " MAC: " << hexMac << " " << decMac << " Not Found" << endl;
			}

		}


		// TODO: Just for debug...
		/*
		for(unsigned int i = 0; i < m_cm_list.size(); i++) {
			m_cm_list[i]->dumpCmCpeList();
		}
		*/
	}


	return true;
}

/**
 * Collect Snmp for CMTS IFs from CMTS (for ex. SNR, RxPwr, TO, MR...)
 */
bool CCmts::collectCmtsIfSnmpBulk() {

	unsigned int i;
	unsigned int j;

	vector <string> listIfId;
	vector <string> listSnmpValues;

	//LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	initCmtsIfTable();
 
	for(i = 0; i < m_snmp_oid_cmts_if.size(); i++) {

		// Forked from here...
	//	int pidChild = fork();

	//	if (pidChild == 0) {
			CSnmpOid& rSnmpOid = m_snmp_oid_cmts_if[i];

			if (m_Ip->SnmpWalkBulk(rSnmpOid.getSnmpOid(), m_cmts_rcommunity,
					&listIfId, &listSnmpValues)) {

				// Add the SNMP Value to the IF in the list
				for(j = 0; j < listIfId.size(); j++) {

					CCmtsIf* cmtsIf = getIfById(listIfId[j]);

					if (cmtsIf != NULL) {
						cmtsIf->setSnmpValue(rSnmpOid, listSnmpValues[j]);
					} else {
						LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":" << listIfId[j] << " Not Found!" << endl;
					}

				}

				CMySqlDB* pDb = m_dbFactory->newMySqlConn();

				m_tools_sql.insertSnmpValuesByIfId(	pDb,
													rSnmpOid,
													listIfId,
													listSnmpValues);

				delete pDb;
			}

	//		break;
//		} else {

	//	}

	}

	return true;
}


void CCmts::setAccountDb(CSqlDbFactory* pDb) {
	m_dbFactory = pDb;
}

void CCmts::setSqlConnPool(CSqlConnPool* pConnPool) {
	m_pDbConnPool = pConnPool;
}

/**
 * Set CMTS ID.
 * TODO: Remove all concrete params and use only param set.
 */
void CCmts::setCmtsId(int cmts_id) {
	m_cmts_id = cmts_id;
	m_tools_sql.setCmtsId(cmts_id);

	this->m_param_set.setParam(KEY_COL_CMTS_ID, cmts_id);
}

void CCmts::setCmtsId(const string& cmts_id) {
	setCmtsId(atoi(cmts_id.c_str()));
}

void CCmts::setCmtsIp(const string& cmts_ip) {

	m_cmts_ip = cmts_ip;

	this->m_param_set.setParam(KEY_COL_CMTS_IP, cmts_ip);

	if (m_Ip != NULL) {
		delete m_Ip;
	}

	m_Ip = new CIp(m_cmts_ip);
}

void CCmts::setCmtsRComunity(const string& cmts_rcommunity) {

	m_cmts_rcommunity = cmts_rcommunity;

	this->m_param_set.setParam(KEY_COL_CMTS_RCOMM, cmts_rcommunity);

	if (m_Ip != NULL) {
		m_Ip->setCommunity(cmts_rcommunity);
	}

}

void CCmts::setCmRComunity(const string& cm_rcommunity) {
	m_cm_rcommunity = cm_rcommunity;
	this->m_param_set.setParam(KEY_COL_CM_RCOMM, cm_rcommunity);
}

void CCmts::setCmtsVer(int cmts_docsis_ver) {
	m_cmts_docsis_ver = cmts_docsis_ver;
	this->m_param_set.setParam(KEY_COL_CMTS_VER, cmts_docsis_ver);
}

void CCmts::setCmtsDesc(const string& cmts_desc) {
	m_cmts_desc = cmts_desc;
	this->m_param_set.setParam(KEY_COL_CMTS_DESC, cmts_desc);
}

void CCmts::setSharedCounter(CSharedCounter* sc) {
	m_pSharedCounter = sc;
}

void CCmts::setCmtsPosition(const int pos) {
	m_cmts_position = pos;
}

void CCmts::setPeriodSleep(int period) {
	m_periodSleep = period;
	this->m_param_set.setParam(KEY_COL_CMTS_PERIOD_SLEEP, period);
}

/**
 * Read from DB CM Errors Table.
 * TODO: Check and test it...
 */
bool CCmts::doCmErrorsList() {

	//LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	stringstream ssSql;

	ssSql << "SELECT * FROM " << TABLE_OSS_CM_ERRORS <<" ";
	ssSql << " WHERE cmts_id = " << m_cmts_id;

	pDb->doSelect(ssSql.str(), m_ds_cm_errors);

	delete pDb;

	m_ds_cm_errors.buildColumnIndex(string(KEY_COL_CM_MAC));
	//m_ds_cm_errors.dumpInfo();

	return true;
}

/**
 * Read from DB CM Errors Table.
 */
bool CCmts::doIfErrorsList() {

        //LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

        CMySqlDB* pDb = m_dbFactory->newMySqlConn();

        stringstream ssSql;

        ssSql << "SELECT * FROM " << TABLE_OSS_IF_ERRORS <<" ";
        ssSql << " WHERE cmts_id = " << m_cmts_id;

        pDb->doSelect(ssSql.str(), m_ds_if_errors);

        delete pDb;

        m_ds_if_errors.buildColumnIndex(string(KEY_COL_IF_ID));
        //m_ds_if_errors.dumpInfo();

        return true;
}

/**
 * Make Full Snmp Oids List
 */
bool CCmts::doSnmpList() {

	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	m_tools_sql.selectOidList(pDb, string(""), m_snmp_oid);

	delete pDb;

	//CSnmpOid::dumpOidList(m_snmp_oid);

	return true;
}

/**
 * Make List with SNMP OIDs for DOCSIS 3.0 CMs from CMTS
 */
bool CCmts::doCmtsCmSnmpDocs3List() {

	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	m_tools_sql.selectCmtsDocs3OidList(pDb, m_snmp_oid_cmts_docs3);

	delete pDb;

	/*
	LOGD << __FUNCTION__ << ": size:" << m_snmp_oid_cmts_docs3.size() << endl;
	for(unsigned int i = 0; i < m_snmp_oid_cmts_docs3.size(); i++) {
		m_snmp_oid_cmts_docs3[i].dumpInfo(); 		
	}
*/

	//CSnmpOid::dumpOidList(m_snmp_oid_cmts_docs3);

	return true;
}

/**
 * Make List with SNMP OIDs for the CMTS
 * TODO: Consider rework...
 */
bool CCmts::doCmtsCmSnmpList() {

	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	m_tools_sql.selectCmtsOidList(pDb, m_snmp_oid_cmts);

	delete pDb;

	return true;
}

/**
 * Make List with SNMP OIDs for the CMTS IFs
 * TODO: Consider rework...
 */
bool CCmts::doCmtsIfSnmpList() {

	//LOGD << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " pid:" << getpid() << endl;

	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	m_tools_sql.selectCmtsIfOidList(pDb, m_snmp_oid_cmts_if);

	delete pDb;

	return true;
}

/**
 * Delete old cm today records.
 */
bool CCmts::doDeleteOldCmToday() {
	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	m_tools_sql.deleteOldCmToday(pDb);

	delete pDb;

	return true;
}

/**
 * Delete old cmts if today records.
 */
bool CCmts::doDeleteOldIfToday() {
	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	m_tools_sql.deleteOldIfToday(pDb);

	delete pDb;

	return true;
}

/**
 * Update Today CM Min Date in StoreTable.
 */
bool CCmts::doUpdateTodayCmMinDate() {
	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	m_tools_sql.updateTodayCmMinDate(pDb);

	delete pDb;

	return true;
}
/**
 * Update Today IF Min Date in StoreTable.
 */
bool CCmts::doUpdateTodayIfMinDate() {
	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	m_tools_sql.updateTodayIfMinDate(pDb);

	delete pDb;

	return true;
}

/**
 * Make List with SNMP OIDs for the direct CM query
 */
bool CCmts::doCmSnmpList() {

	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	m_tools_sql.selectCmOidList(pDb, m_snmp_oid_cm);

	delete pDb;

	return true;
}

/**
 * Initialize CM Table Name - used to store SNMP values.
 */
void CCmts::initCmTable() {

	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	m_tools_sql.initCmTable(pDb);

	delete pDb;
}

/**
 * Initialize CMTS IF Table Name - used to store SNMP values.
 */
void CCmts::initCmtsIfTable() {

	CMySqlDB* pDb = m_dbFactory->newMySqlConn();

	m_tools_sql.initCmtsIfTable(pDb);

	delete pDb;
}

//	Vector CMs List handling functions
/**
 * Add CM to vector List of CMs and make the proper map indexes.
 */
void CCmts::addCmBySid(const string& sid, CCmtsCm* cm) {

	int idx = m_cm_list.size();

	m_map_idx_sid[sid] = idx;
	m_map_idx_mac[cm->getCmMac()] = idx;

	m_cm_list.push_back(cm);
}

CCmtsCm* CCmts::getCmBySid(const string& key) {

	int idx;

	m_it_idx = m_map_idx_sid.find(key);

	if (m_it_idx != m_map_idx_sid.end()) {
		idx = m_it_idx->second;
		return m_cm_list[idx];
	} else {
		return NULL;
	}
}

CCmtsCm* CCmts::getCmByMac(const string& key) {

	int idx;

	m_it_idx = m_map_idx_mac.find(key);

	if (m_it_idx != m_map_idx_mac.end()) {
		idx = m_it_idx->second;
		return m_cm_list[idx];
	} else {
		return NULL;
	}
}

/**
 * Set CM Param searched by SID
 */
bool CCmts::setCmParamBySid(const string& sid, const string& key, const string& val) {
	CCmtsCm* cm = getCmBySid(sid);

	if (cm != NULL) {
		cm->setParam(key, val);
	} else {
		return false;
	}

	return true;
}
/**
 * Add CMTS IF by If Id
 */
void CCmts::addIfById(const string& if_id) {
	CCmtsIf* cmtsIf;

	cmtsIf = new CCmtsIf(m_cmts_id);

	addIfById(if_id, cmtsIf);

}
void CCmts::addIfById(const string& if_id, CCmtsIf* cmtsIf) {

	int idx = m_if_list.size();

	cmtsIf->setParam(KEY_SNMP_OID_IF_ID, if_id);

	m_map_idx_if[if_id] = idx;

	m_if_list.push_back(cmtsIf);

}

CCmtsIf* CCmts::getIfById(const string& key) {
	int idx;

	m_it_idx_if = m_map_idx_if.find(key);

	if (m_it_idx_if != m_map_idx_if.end()) {
		idx = m_it_idx_if->second;
		return m_if_list[idx];
	} else {
		return NULL;
	}
}

bool CCmts::setIfParamById(const string& if_id, const string& key, const string& val) {
	CCmtsIf* cmtsIf = getIfById(if_id);

	if (cmtsIf != NULL) {
		cmtsIf->setParam(key, val);
	} else {
		return false;
	}

	return true;
}

/**
 * Clear CM List
 */
void CCmts::clearCmList() {
	unsigned int i;

	for(i = 0; i < m_cm_list.size(); i++) {
		delete m_cm_list[i];
	}

	m_cm_list.clear();
}

/**
 * Clear IF List
 */
void CCmts::clearIfList() {
	unsigned int i;

	for(i = 0; i < m_if_list.size(); i++) {
		delete m_if_list[i];
	}

	m_if_list.clear();
}

size_t CCmts::getCmCount() {
	return m_cm_list.size();
}

/**
 * Dump CMTS Info (for Debug)
 */
void CCmts::dumpCmtsInfo() {
	LOGD << "PID: " << getpid() << " ";
	LOGD << "[" << m_cmts_id << "] ";
	LOGD << "[" << m_cmts_ip << "] ";
	LOGD << "[" << m_cmts_rcommunity << "] ";
	LOGD << "[" << m_cm_rcommunity << "] ";
	LOGD << "[" << m_cmts_desc << "] ";
	LOGD << m_cmts_docsis_ver << endl;

	//CSnmpOid::dumpOidList(m_snmp_oid_cmts);
	//CSnmpOid::dumpOidList(m_snmp_oid_cm);
	//CSnmpOid::dumpOidList(m_snmp_oid_cmts_if);

}
void CCmts::dumpCmList() {
	unsigned int i;

	for(i = 0; i < m_cm_list.size(); i++) {
		m_cm_list[i]->dumpCmInfo();
	}

}
void CCmts::dumpIfList() {
	unsigned int i;

	for (i = 0; i < m_if_list.size(); i++) {
		m_if_list[i]->dumpInfo();
	}
}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */
