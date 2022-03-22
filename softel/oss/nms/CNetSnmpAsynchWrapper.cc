/*
 * CNetSnmpAsynchWrapper.cc
 *
 *  Created on: 27.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CNetSnmpAsynchWrapper.h"

#include <softel/core/error/CErrorCommon.h>

using namespace std;
using namespace SF::OSS::CMTS;
using namespace SF::CORE::SYSTEM;
using namespace SF::CORE::ERROR;

namespace SF {
namespace OSS {
namespace NMS {

#define MAX_RETRIES 2
#define SLEEP_NETELEM_WAIT 50000


struct session {
	struct snmp_session *sess; 	// SNMP session data

	unsigned int oid_idx; 		// Index of current OID to query
	unsigned int retries;		// Num of retries if there is no answer
	unsigned int max_retries;	// Num of max retries

	BNetworkElementBase* 		pNetElem;
	std::vector<CSnmpOid>* 	pOidList;

	//std::vector<CSnmpValue>	resultSnmpValueList;
	//CSnmpValueSet	setSnmpValue;

	int*	pNumActiveHosts;
	pthread_mutex_t* pMutex;
};


CNetSnmpAsynchWrapper::CNetSnmpAsynchWrapper() {
	initialize();
}

CNetSnmpAsynchWrapper::~CNetSnmpAsynchWrapper() {
	// TODO Auto-generated destructor stub
}

void CNetSnmpAsynchWrapper::initialize(void) {
	/* initialize library */
	init_snmp("asynchapp");

	pthread_mutex_init(&m_mutex, NULL);
}

/**
 * Process Result from asynch snmp get
 * TODO: Make some C-style rework...
 */
int CNetSnmpAsynchWrapper::processResult(int status, struct snmp_session *sp,
		struct snmp_pdu *pdu, char* snmpResult) {

	char buf[32768];
	struct variable_list *vp;
	int ix;

	switch (status) {
	case STAT_SUCCESS:
		vp = pdu->variables;
		if (pdu->errstat == SNMP_ERR_NOERROR) {

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
					std::cout << __FILE__ << ":" << __FUNCTION__ << " :pid:" << CSystemTools::doGetPid() << ":ERROR:Empty snmpResult: BUF:" << buf << std::endl;
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

			std::cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << CSystemTools::doGetPid() << " :ERROR: " << std::endl;
		//	std::cout << sp->peername << ":" << buf << ":" << snmp_errstring(pdu->errstat) << std::endl;


		}

		//cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << CSystemTools::doGetPid() << ":" << sp->peername << " BUF["  << buf << "] snmpResult[" << snmpResult << "]" << endl;

		return 1;

	case STAT_TIMEOUT:
		std::cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << CSystemTools::doGetPid() << " :STAT_TIMEOUT: " << std::endl;
		//cout << sp->peername << endl;
		//fprintf(stdout, "%s: Timeout\n", sp->peername);
		return 0;
	case STAT_ERROR:
		std::cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << CSystemTools::doGetPid() << " :STAT_ERROR: " << std::endl;
		//cout << sp->peername << endl;
		snmp_perror(sp->peername);
		return 0;
	}

	//cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << CSystemTools::doGetPid() << "BUF["  << buf << "] snmpResult[" << snmpResult << "]" << endl;

	return 0;
}

int CNetSnmpAsynchWrapper::asynchResponse(int operation, struct snmp_session *sp,
		int reqid, struct snmp_pdu *pdu, void *magic) {

	struct session *host = (struct session *) magic;
	struct snmp_pdu *req;

	char snmpResult[32768] = {0};

	oid id_oid[MAX_OID_LEN];
	size_t id_len = MAX_OID_LEN;

	vector<CSnmpOid>& rOidList = *(host->pOidList);

	cout << __FILE__ << ":" << __FUNCTION__ << " :pid:" << CSystemTools::doGetPid() << " : " << host->pNetElem->getIp() << " oidIdx=" << host->oid_idx << "/" << host->pOidList->size();
	cout << " oid = " << rOidList[host->oid_idx].getSnmpOid() << " opres:" << operation << " opok:" << NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE << endl;

	if (operation == NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE) {
		if (processResult(STAT_SUCCESS, host->sess, pdu, snmpResult)) {

			// Save the snmp result
			if(strlen(snmpResult) > 0) {

				// Save into Snmp Result Vector for corresponding Oid
				CSnmpOid currentSnmpOid = rOidList[host->oid_idx];
				//CSnmpValue currentSnmpValue(currentSnmpOid, snmpResult);
				//host->resultSnmpValueList.push_back(currentSnmpValue);
				//host->setSnmpValue.setSnmpValue(currentSnmpOid, snmpResult);
				host->pNetElem->getSnmpValueSet().setSnmpValue(currentSnmpOid, snmpResult);

			} else {
				cout << __FILE__ << ":" << __FUNCTION__ << " :pid:" << CSystemTools::doGetPid() << ":ERROR:Empty snmpResult" << endl;
			}

			host->oid_idx++; /* send next GET (if any) */

			if (host->oid_idx < host->pOidList->size()) {

				req = snmp_pdu_create(SNMP_MSG_GET);

				CSnmpOid snmpOid = rOidList[host->oid_idx];

				if (!read_objid(snmpOid.getSnmpOid().c_str(), id_oid, &id_len)) {
					throw CErrorCommon(std::string("Error: ") + __FUNCTION__);
				}

				snmp_add_null_var(req, id_oid, id_len);

				if (snmp_send(host->sess, req))
					return 1;
				else {
					snmp_perror("snmp_send");
					snmp_free_pdu(req);
				}
			} else {
				std::cout << __FILE__ << ":" << __FUNCTION__ << " :pid:" << CSystemTools::doGetPid() << " : " << host->pNetElem->getIp();
				std::cout << " oidIdx=" << host->oid_idx << "/" << host->pOidList->size() << " CMASYNCH FINISHED" << std::endl;
			}

		}
	} else {
		if (host->retries > 0) {
			host->retries--;

			// TODO: Rework here, consider code reuse...

			if (host->oid_idx < host->pOidList->size()) {

                                // Make some pause (modems sometimes do not response if it is too fast...)
                                CSystemTools::doUSleep(SLEEP_NETELEM_WAIT);

                                req = snmp_pdu_create(SNMP_MSG_GET);

                                CSnmpOid snmpOid = rOidList[host->oid_idx];

				std::cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" <<  CSystemTools::doGetPid() << " : " << host->pNetElem->getIp() << ":Retry:" << snmpOid.getSnmpOid() << std::endl;

                                if (!read_objid(snmpOid.getSnmpOid().c_str(), id_oid, &id_len)) {
                                        throw CErrorCommon(std::string("Error: ") + __FUNCTION__);
                                }

                                snmp_add_null_var(req, id_oid, id_len);

                                if (snmp_send(host->sess, req))
                                        return 1;
                                else {
					std::cout << __FILE__ << ":" << __FUNCTION__ << " :pid:" << CSystemTools::doGetPid() << ":ERROR:snmp_send" << std::endl;

                                        snmp_perror("snmp_send");
                                        snmp_free_pdu(req);
                                }
                        } else {

				cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" <<  CSystemTools::doGetPid() << " : " << host->pNetElem->getIp() << ":OidList Ended" << std::endl;

			}

		} else {

			std::cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" <<  CSystemTools::doGetPid() << " : " << host->pNetElem->getIp() << ":Retry Ended" << std::endl;
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



/**
 * Decrement Number of Active Hosts. Serialized by mutex.
 */
void CNetSnmpAsynchWrapper::decNumActiveHosts(int* pActiveHosts, pthread_mutex_t* mutex) {

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
 * Asynch Multiple Snmp Gets from multiple Hosts.
 * List of CMs and SnmpOids are provided.
 * Here should be processed and the result.
 * TODO: May be result should be returned.
 */
bool CNetSnmpAsynchWrapper::snmpMultipleGetAsynch(	vector<BNetworkElementBase*>& listNetElem, vector<CSnmpOid>& oidList) {

	cout << __FILE__ << ":" << __FUNCTION__ << " pid:" << CSystemTools::doGetPid() << " size:" << listNetElem.size() << " Started" << endl;

	struct session* pHs[listNetElem.size()];

	unsigned int i;

	oid id_oid[MAX_OID_LEN];
	size_t id_len = MAX_OID_LEN;

	// Check if there are CMs and Oids
	if (listNetElem.size() == 0 || oidList.size() == 0) {
		return false;
	}

	//strcpy(rcomm, rcommunity.c_str());

	m_active_hosts = 0;

	for (i = 0; i < listNetElem.size(); i++) {

		pHs[i] = new struct session;

		// Check for valid net elem ip
		if (listNetElem[i]->isActive() == false) {

			cout << __FUNCTION__ << ":pid:" << CSystemTools::doGetPid() << "  SKIP ip:" << listNetElem[i]->getIp() << endl;

			pHs[i]->pNetElem = 0;

			continue;
		}


		cout << __FUNCTION__ << ":pid:" << CSystemTools::doGetPid() << "  CRAWL ip:" << listNetElem[i]->getIp() << endl;

		char rcomm[64];
		char hostIp[32];

		strcpy(rcomm, listNetElem[i]->getSnmpRComm().c_str());
		strcpy(hostIp, listNetElem[i]->getIp().c_str());

		struct snmp_pdu *req;
		struct snmp_session sess;
		snmp_sess_init(&sess); /* initialize session */
		sess.version = SNMP_VERSION_2c;
		sess.peername = hostIp;
		sess.community = (u_char*) rcomm;
		sess.community_len = strlen((char*)sess.community);
		sess.callback = CNetSnmpAsynchWrapper::asynchResponse; 					/* default callback */
		sess.callback_magic = pHs[i];

		cout << __FUNCTION__ << "::" << __LINE__ << " pid:" << CSystemTools::doGetPid();
		cout << " SESS: " << " sess.peername: " << sess.peername;
		cout << " sess.version: " << sess.version;
		cout << " sess.community: " << sess.community << " sess.community_len: " << sess.community_len;
		cout << " sess.version: " << sess.version << endl;

		if (!(pHs[i]->sess = snmp_open(&sess))) {

			cout << __FUNCTION__ << ":pid:" << CSystemTools::doGetPid() << "ERROR: snmp_open ip:" << listNetElem[i]->getIp() << endl;

			snmp_perror("snmp_open");

			continue;
		}

		pHs[i]->oid_idx = 0;
		pHs[i]->retries = MAX_RETRIES;
		pHs[i]->pNetElem = listNetElem[i];
		pHs[i]->pOidList = &oidList;
		pHs[i]->pNumActiveHosts = &m_active_hosts;
		//pHs[i]->resultSnmpValueList.clear();
		//pHs[i]->setSnmpValue.clear();
		pHs[i]->pMutex = &m_mutex;

		if (!read_objid(oidList[0].getSnmpOid().c_str(), id_oid, &id_len)) {
			throw CErrorCommon(string("Error:") + __FUNCTION__);
		}
		req = snmp_pdu_create(SNMP_MSG_GET); /* send the first GET */
		snmp_add_null_var(req, id_oid, id_len);
		if (snmp_send(pHs[i]->sess, req)) {
			m_active_hosts++;
			cout << __FUNCTION__ << ":pid:" << CSystemTools::doGetPid() << " ip:" << listNetElem[i]->getIp() << " snmp_send OK" << endl;
		}
		else {

			cout << __FUNCTION__ << ":pid:" << CSystemTools::doGetPid() << "ERROR: snmp_send ip:" << listNetElem[i]->getIp() << " sleeping..." << endl;
			// TODO: Make it better....
			CSystemTools::doSleep(1);
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
		int fds = 0;
		int block = 1;
		fd_set fdset;
		struct timeval timeout;

		cout << __FUNCTION__ << "::" << __LINE__ << " pid:" << CSystemTools::doGetPid() << " m_active_hosts: " << m_active_hosts << endl;

		netsnmp_large_fd_set lfdset;

		cout << __FUNCTION__ << "::" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;
		FD_ZERO(&fdset);
		cout << __FUNCTION__ << "::" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;
		snmp_select_info2(&fds, &lfdset, &timeout, &block);
		cout << __FUNCTION__ << "::" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;
		fds = select(fds, lfdset.lfs_setptr, NULL, NULL, block ? NULL : &timeout);
		cout << __FUNCTION__ << "::" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;
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

		cout << __FUNCTION__ << ": Active: " << m_active_hosts << endl;

		// Section Handling remaining active hosts which are not processed...
		// Reason can be wrong active hosts count or not responding hosts...
		if (prevActiveHosts == m_active_hosts) {
			numDupActiveHosts--;

			//cout << __FUNCTION__ << ":pid:" << CSystemTools::doGetPid() << " m_active_hosts:" << m_active_hosts << " numDupActiveHosts:" << numDupActiveHosts << endl;
			if (numDupActiveHosts == 0) {
				break;
			}

			//sleep(1);

		} else {
			numDupActiveHosts = maxDupActiveHostsNum;
		}

		cout << __FUNCTION__ << "::" << __LINE__ << " pid:" << CSystemTools::doGetPid() << endl;
		prevActiveHosts = m_active_hosts;

		//usleep(200000);
		CSystemTools::doUSleep(20000);
	}

	// Save the collected Snmp Results List
	//saveSnmpValues(pHs, cmList.size());
	finalize(pHs, listNetElem.size());

	cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << CSystemTools::doGetPid() << " Ended with NumActiveHosts:" << m_active_hosts << endl;

	return true;
}

bool CNetSnmpAsynchWrapper::snmpMultipleGetAsynch(
		CNetworkElementList& listNetElem, vector<CSnmpOid>& oidList,
		const int maxCmCount) {

	vector<BNetworkElementBase*> currentListNetElem;
	currentListNetElem.clear();

	unsigned int i;
	int k;

	for (i = 0, k = 0; i < listNetElem.size(); i++) {

		currentListNetElem.push_back(listNetElem.get(i));

		if (k >= maxCmCount) {

			snmpMultipleGetAsynch(currentListNetElem, oidList);

			currentListNetElem.clear();

			k = 0;

		} else {
			k++;
		}

	}

	snmpMultipleGetAsynch(currentListNetElem, oidList);

	return true;
}

/**
 * Finalize snmp asynch query
 */
bool CNetSnmpAsynchWrapper::finalize(struct session* hs[], int hsSize) {

	int i;
	unsigned int j;

	cout << __FILE__ << ":" << __FUNCTION__ << "pid:" << getpid() << endl;

	for (i = 0; i < hsSize; i++) {

		if (hs[i]->pNetElem != 0) {

			if (hs[i]->sess) {
				snmp_close(hs[i]->sess);
			}


		}

		delete hs[i];
	}

	cout << __FILE__ << ":" << __FUNCTION__ << "pid:" << getpid() << " Ended" << endl;

	return true;
}

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */



