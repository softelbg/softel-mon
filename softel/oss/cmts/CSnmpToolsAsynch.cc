/*
 * CSnmpToolsAsynch.cc
 *
 *	SNMP Asynch requests
 *
 *  Created on: 03.11.2011
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSnmpToolsAsynch.h"

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::NET;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace OSS {
namespace CMTS {

#define MAX_RETRIES 2
#define SLEEP_MODEM_WAIT 50000

  
struct session {
	struct snmp_session *sess; 	// SNMP session data 

	unsigned int oid_idx; 		// Index of current OID to query 
	unsigned int retries;		// Num of retries if there is no answer 	

	CCmtsCm* 			pCm;

	// Lists of Snmp Oids and corresponding SnmpValues Results
	vector<CSnmpOid>* 	pOidList;
	vector<CSnmpValue>	resultSnmpValueList;

	int*	pNumActiveHosts;
	pthread_mutex_t* pMutex;
};

CSnmpToolsAsynch::CSnmpToolsAsynch() {

	initialize();

}
CSnmpToolsAsynch::CSnmpToolsAsynch(const string& ip) {

	initialize();

	setIp(ip);
}

CSnmpToolsAsynch::~CSnmpToolsAsynch() {

}

void CSnmpToolsAsynch::setIp(const string& ip) {
	m_ip = ip;
}

/**
 * Decrement Number of Active Hosts. Serialized by mutex.
 */
void CSnmpToolsAsynch::decNumActiveHosts(int* pActiveHosts, pthread_mutex_t* mutex) {
	
	//critical section
	pthread_mutex_lock(mutex);

	int numHosts = *(pActiveHosts);

	numHosts--;

	*(pActiveHosts) = numHosts;

	//cout << __FUNCTION__ << ": Active Hosts:" << *(pActiveHosts) << endl;

	pthread_mutex_unlock(mutex);
	//end of critical section

}

/**
 * TODO: Better use only MAP for CM List...
 * Skip this map->vector copy of CMs
 */
bool CSnmpToolsAsynch::snmpMultipleGetAsynch(	map<string, CCmtsCm*>& cmMap,
												vector<CSnmpOid>& oidList) {

	map<string, CCmtsCm*>::iterator it;
	vector<CCmtsCm*> cmVector;

	cmVector.clear();

	int nCm = 0;

	cout << __FILE__ << ":" << __FUNCTION__ << "pid:" << getpid() << "size:" << cmMap.size() << endl;
	
	for (it = cmMap.begin(); it != cmMap.end(); it++) {
			CCmtsCm* cm = it->second;

			//cm->dumpCmInfo();

			const string& cmStatus = cm->getParam(KEY_COL_CM_STATUS);

			if (atoi(cmStatus.c_str()) == STATUS_CM_REGISTRATION_COMPLETE) {
				cmVector.push_back(cm);

				nCm++;
				if (nCm >= 3000) {
					cout << "inSize=" << cmMap.size() << " outSize=" << cmVector.size() << endl;
					snmpMultipleGetAsynch(cmVector, oidList);
					nCm = 0;
					cmVector.clear();
				}
			}
	}

	cout << __FILE__ << ":" << __FUNCTION__ << "pid:" << getpid() << " Ended outSize=" << cmVector.size() << endl;
	cout << "inSize=" << cmMap.size() << " outSize=" << cmVector.size() << endl;

	snmpMultipleGetAsynch(cmVector, oidList);

	return true;
}

bool CSnmpToolsAsynch::snmpMultipleGetAsynch(
								vector<CCmtsCm*>& cmList,
								vector<CSnmpOid>& oidList,
								const int maxCmCount) {

	vector<CCmtsCm*> currentCmList;
	unsigned int i;
	int k;

	currentCmList.clear();

	for(i = 0, k = 0; i < cmList.size(); i++) {

		currentCmList.push_back(cmList[i]);

		if (k >= maxCmCount) {

			snmpMultipleGetAsynch(currentCmList, oidList);

			currentCmList.clear();
			
			k = 0;

		} else {
			k++;
		}

	}

	snmpMultipleGetAsynch(currentCmList, oidList);

	return true;
}

/**
 * Asynch Multiple Snmp Gets from multiple Hosts.
 * List of CMs and SnmpOids are provided.
 * Here should be processed and the result.
 * TODO: May be result should be returned.
 */
bool CSnmpToolsAsynch::snmpMultipleGetAsynch(	vector<CCmtsCm*>& cmList,
												vector<CSnmpOid>& oidList) {

	cout << __FILE__ << ":" << __FUNCTION__ << " pid:" << getpid() << " size:" << cmList.size() << " Started" << endl;

	struct session* pHs[cmList.size()];

	char rcomm[64];
	char hostIp[32];
	
	unsigned int i;

	oid id_oid[MAX_OID_LEN];
	size_t id_len = MAX_OID_LEN;

	// Check if there are CMs and Oids
	if (cmList.size() == 0 || oidList.size() == 0) {
		return false;
	}

	//strcpy(rcomm, rcommunity.c_str());

	m_active_hosts = 0;

	for (i = 0; i < cmList.size(); i++) {

		pHs[i] = new struct session;

		// Check is CM active. TODO: Consider avoid new memory for non active...
		if (cmList[i]->isActive() == false || cmList[i]->isValidIp() == false) {

			//cout << __FUNCTION__ << ":pid:" << getpid() << "  SKIP ip:" << cmList[i]->getCmIp() << endl;
 
			pHs[i]->pCm = 0;

			continue;
		}


		//cout << __FUNCTION__ << ":pid:" << getpid() << "  CRAWL ip:" << cmList[i]->getCmIp() << endl;

		strcpy(rcomm, cmList[i]->getRCommunityCm().c_str());
		strcpy(hostIp, cmList[i]->getCmIp().c_str());

		struct snmp_pdu *req;
		struct snmp_session sess;
		snmp_sess_init(&sess); /* initialize session */
		sess.version = SNMP_VERSION_2c;
		sess.peername = hostIp;
		sess.community = (u_char*) rcomm;
		sess.community_len = strlen((char*)sess.community);
		sess.callback = CSnmpToolsAsynch::asynchResponse; 					/* default callback */
		sess.callback_magic = pHs[i];

		if (!(pHs[i]->sess = snmp_open(&sess))) {

			cout << __FUNCTION__ << ":pid:" << getpid() << "ERROR: snmp_open ip:" << cmList[i]->getCmIp() << endl;
  
			snmp_perror("snmp_open");
	
			continue;
		}

		pHs[i]->oid_idx = 0;
		pHs[i]->retries = MAX_RETRIES;
		pHs[i]->pCm = cmList[i];
		pHs[i]->pOidList = &oidList;
		pHs[i]->pNumActiveHosts = &m_active_hosts;
		pHs[i]->resultSnmpValueList.clear();
		pHs[i]->pMutex = &m_mutex;

		if (!read_objid(oidList[0].getSnmpOid().c_str(), id_oid, &id_len)) {
			throw CErrorCommon(string("Error:") + __FUNCTION__);
		}
		req = snmp_pdu_create(SNMP_MSG_GET); /* send the first GET */
		snmp_add_null_var(req, id_oid, id_len);
		if (snmp_send(pHs[i]->sess, req)) {
			m_active_hosts++;
			//cout << __FUNCTION__ << ":pid:" << getpid() << " ip:" << cmList[i]->getCmIp() << " snmp_send OK" << endl;
		}
		else {

			cout << __FUNCTION__ << ":pid:" << getpid() << "ERROR: snmp_send ip:" << cmList[i]->getCmIp() << " sleeping..." << endl;
			// TODO: Make it better....
			sleep(1);
			if (snmp_send(pHs[i]->sess, req))
        	                m_active_hosts++;
 	                else {

						
			snmp_perror("snmp_send");
			snmp_free_pdu(req);
			
			}
		}

		
	}

	/* loop while any active hosts */
	int maxDupActiveHostsNum = 50;
	int numDupActiveHosts = maxDupActiveHostsNum;
	int prevActiveHosts = m_active_hosts;

	while (m_active_hosts) {
		int fds = 0, block = 1;
		fd_set fdset;
		struct timeval timeout;

		netsnmp_large_fd_set lfdset;

		FD_ZERO(&fdset);

		snmp_select_info2(&fds, &lfdset, &timeout, &block);

		fds = select(fds, lfdset.lfs_setptr, NULL, NULL, block ? NULL : &timeout);

		if (fds < 0) {
			throw CErrorCommon(string("Select Failed: ") + __FUNCTION__);
		}
		if (fds) {
			snmp_read2(&lfdset);
		}
		else {
			cout << __FUNCTION__ << ": Snmp Timeout" << endl;
			snmp_timeout();
		}

		//cout << __FUNCTION__ << ": Active: " << m_active_hosts << endl;

		// Section Handling remaining active hosts which are not processed...
		// Reason can be wrong active hosts count or not responding hosts...
		if (prevActiveHosts == m_active_hosts) {
			numDupActiveHosts--;

			//cout << __FUNCTION__ << ":pid:" << getpid() << " m_active_hosts:" << m_active_hosts << " numDupActiveHosts:" << numDupActiveHosts << endl;
			if (numDupActiveHosts == 0) {
				break;
			}

			//sleep(1);

		} else {
			numDupActiveHosts = maxDupActiveHostsNum;
		}

		prevActiveHosts = m_active_hosts;

		//usleep(200000);
		usleep(20000);
	}

	// Save the collected Snmp Results List
	saveSnmpValues(pHs, cmList.size());

	cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid() << " Ended with NumActiveHosts:" << m_active_hosts << endl;

	return true;
}

/**
 * Itterate all CMs and all their Snmp Results and save them as CM Params
 */
bool CSnmpToolsAsynch::saveSnmpValues(struct session* hs[], int hsSize) {

	int i;
	unsigned int j;

	cout << __FILE__ << ":" << __FUNCTION__ << "pid:" << getpid() << endl;

	for (i = 0; i < hsSize; i++) {

		if (hs[i]->pCm != 0) {

			if (hs[i]->sess) {
				snmp_close(hs[i]->sess);
			}
			
			vector<CSnmpValue>& rSnmpValueList = hs[i]->resultSnmpValueList;

			for(j = 0; j < rSnmpValueList.size(); j++) {
				hs[i]->pCm->setParam(rSnmpValueList[j].getSnmpOid(), rSnmpValueList[j].getRawValue());
			}
		}

		delete hs[i];
	}

cout << __FILE__ << ":" << __FUNCTION__ << "pid:" << getpid() << " Ended" << endl;

	return true;
}

/*
 * initialize
 */
void CSnmpToolsAsynch::initialize(void) {

	/* initialize library */
	init_snmp("asynchapp");

	pthread_mutex_init(&m_mutex, NULL);
}

/*
 * Process Result from asynch snmp get
 */
int CSnmpToolsAsynch::processResult(int status, struct snmp_session *sp,
		struct snmp_pdu *pdu, char* snmpResult) {

	char buf[32768];
	struct variable_list *vp;
	int ix;

	switch (status) {
	case STAT_SUCCESS:
		vp = pdu->variables;
		if (pdu->errstat == SNMP_ERR_NOERROR) {

			// TODO: PRocess result here - should save CM snmp result
			while (vp) {
				snprint_variable(buf, sizeof(buf), vp->name, vp->name_length,
						vp);
				
				// Copy SnmpResult
				// TODO: very dirty... consider better snmp result parse...
				char* cp;
				cp = strstr(buf, ": ");
				if (cp) {
					strcpy(snmpResult, cp+2);
				} else {
					//strcpy(snmpResult, buf);
					strcpy(snmpResult, "");
					cout << __FILE__ << ":" << __FUNCTION__ << " :pid:" << getpid() << ":ERROR:Empty snmpResult: BUF:" << buf << endl;
				}

				//fprintf(stdout, "%s: %s\n", sp->peername, buf);
					
				vp = vp->next_variable;
			}
		} else {
			for (ix = 1; vp && ix != pdu->errindex; vp = vp->next_variable, ix++)
				;
			if (vp) {
				snprint_objid(buf, sizeof(buf), vp->name, vp->name_length);
			} else
				strcpy(buf, "(none)");

			cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid() << " :ERROR: " << endl;
		//	cout << sp->peername << ":" << buf << ":" << snmp_errstring(pdu->errstat) << endl;


		}

		//cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid() << ":" << sp->peername << " BUF["  << buf << "] snmpResult[" << snmpResult << "]" << endl;

		return 1;

	case STAT_TIMEOUT:
		cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid() << " :STAT_TIMEOUT: " << endl;
		//cout << sp->peername << endl; 
		//fprintf(stdout, "%s: Timeout\n", sp->peername);
		return 0;
	case STAT_ERROR:
		cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid() << " :STAT_ERROR: " << endl;
		//cout << sp->peername << endl;
		snmp_perror(sp->peername);
		return 0;
	}

	//cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid() << "BUF["  << buf << "] snmpResult[" << snmpResult << "]" << endl;

	return 0;
}

int CSnmpToolsAsynch::asynchResponse(int operation, struct snmp_session *sp,
		int reqid, struct snmp_pdu *pdu, void *magic) {

	struct session *host = (struct session *) magic;
	struct snmp_pdu *req;

	char snmpResult[32768] = {0};

	oid id_oid[MAX_OID_LEN];
	size_t id_len = MAX_OID_LEN;

	vector<CSnmpOid>& rOidList = *(host->pOidList);

	//cout << __FILE__ << ":" << __FUNCTION__ << " :pid:" << getpid() << " : " << host->pCm->getCmIp() << " oidIdx=" << host->oid_idx << "/" << host->pOidList->size();
	//cout << " oid = " << rOidList[host->oid_idx].getSnmpOid() << " opres:" << operation << " opok:" << NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE << endl;
	
	if (operation == NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE) {
		if (processResult(STAT_SUCCESS, host->sess, pdu, snmpResult)) {

			// Save the snmp result
			if(strlen(snmpResult) > 0) {

				// Save into Snmp Result Vector for corresponding Oid
				CSnmpOid currentSnmpOid = rOidList[host->oid_idx];
				CSnmpValue currentSnmpValue(currentSnmpOid, snmpResult);
				host->resultSnmpValueList.push_back(currentSnmpValue);

			} else {
				//cout << __FILE__ << ":" << __FUNCTION__ << " :pid:" << getpid() << ":ERROR:Empty snmpResult" << endl; 
			}

			host->oid_idx++; /* send next GET (if any) */

			if (host->oid_idx < host->pOidList->size()) {

				req = snmp_pdu_create(SNMP_MSG_GET);

				CSnmpOid snmpOid = rOidList[host->oid_idx];
				
				if (!read_objid(snmpOid.getSnmpOid().c_str(), id_oid, &id_len)) {
					throw CErrorCommon(string("Error: ") + __FUNCTION__);
				}

				snmp_add_null_var(req, id_oid, id_len);
	
				if (snmp_send(host->sess, req))
					return 1;
				else {
					snmp_perror("snmp_send");
					snmp_free_pdu(req);
				}
			} else {
				cout << __FILE__ << ":" << __FUNCTION__ << " :pid:" << getpid() << " : " << host->pCm->getCmIp();
				cout << " oidIdx=" << host->oid_idx << "/" << host->pOidList->size() << " CMASYNCH FINISHED" << endl;
			}

		}
	} else {
		if (host->retries > 0) {
			host->retries--;

			// TODO: Rework here, consider code reuse... 

			if (host->oid_idx < host->pOidList->size()) {

                                // Make some pause (modems sometimes do not response if it is too fast...)
                                usleep(SLEEP_MODEM_WAIT);

                                req = snmp_pdu_create(SNMP_MSG_GET);

                                CSnmpOid snmpOid = rOidList[host->oid_idx];

				cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" <<  getpid() << " : " << host->pCm->getCmIp() << ":Retry:" << snmpOid.getSnmpOid() << endl;

                                if (!read_objid(snmpOid.getSnmpOid().c_str(), id_oid, &id_len)) {
                                        throw CErrorCommon(string("Error: ") + __FUNCTION__);
                                }

                                snmp_add_null_var(req, id_oid, id_len);

                                if (snmp_send(host->sess, req))
                                        return 1;
                                else {
					cout << __FILE__ << ":" << __FUNCTION__ << " :pid:" << getpid() << ":ERROR:snmp_send" << endl;

                                        snmp_perror("snmp_send");
                                        snmp_free_pdu(req);
                                }
                        } else {
				
				cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" <<  getpid() << " : " << host->pCm->getCmIp() << ":OidList Ended" << endl;
			
			}		

		} else {

			cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" <<  getpid() << " : " << host->pCm->getCmIp() << ":Retry Ended" << endl;
			processResult(STAT_TIMEOUT, host->sess, pdu, snmpResult);

		}

	}


	/* something went wrong (or end of variables)
	 * this host not active any more
	 */


	decNumActiveHosts(host->pNumActiveHosts, host->pMutex);

	//cout << __FUNCTION__ << ": Active Hosts:" << *(host->pNumActiveHosts) << endl;

	return 1;
}


} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */
