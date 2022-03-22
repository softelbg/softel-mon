

#include "CIp.h"

#include <softel/core/util/CStringUtil.h>

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::ERROR;
using namespace SF::CORE::UTIL;

namespace SF {
namespace CORE {
namespace NET {

pthread_mutex_t CIp::mutex;

CIp::CIp() {

}
CIp::CIp(const char *pIp) {
	setIp(pIp);
}

CIp::CIp(const std::string& ip) {
	setIp(ip);
}

CIp::CIp(const std::string& ip, const std::string& community) {
	setIp(ip);
	setCommunity(community);
}

CIp::~CIp() {

/*
	if (m_fPingSrc)
		fclose(m_fPingSrc);
	if (m_fPingRes)
		fclose(m_fPingRes);
*/

}
;

void CIp::setIp(const std::string& ip) {
	setIp(ip.c_str());
}

void CIp::setIp(const char *pIp) {

	strcpy(m_Ip, pIp);

	m_SnmpResult[0] = 0;
/*
	m_fPingSrc = fopen("/tmp/fpingsrc.txt", "w");
	m_fPingRes = NULL;
	if (!m_fPingSrc)
		throw CErrorCommon(FEOPENFPINGSRC);
*/
}

bool CIp::Ping(unsigned int &rcv, unsigned int &avg, unsigned int PacketSize) {
	char command[100], returnValue[100];
	FILE * shellPipe;
	char * pch;

	sprintf(command, "/usr/sbin/fping -c %u -b %u -p 1 -q %s 2>&1", rcv,
			PacketSize, m_Ip);
	//printf("%s\n",command);
	if ((shellPipe = popen(command, "r")) == NULL)
		throw CErrorCommon(FEEXECFPING);
	if (fread(returnValue, sizeof(char), 100, shellPipe) <= 1)
		return false;
	//printf("%s - %s\n", m_Ip, returnValue);
	strtok(returnValue, "/");
	strtok(NULL, "/");
	strtok(NULL, "/");
	pch = strtok(NULL, "/");
	rcv = atoi(pch);

	if (rcv > 0) {
		strtok(NULL, "/");
		strtok(NULL, "/");
		strtok(NULL, "/");
		pch = strtok(NULL, "/");
		avg = atoi(pch) + 1;
	} else
		avg = 0;

	//printf("IP:%s - Rcv:%i, Avg:%ims\n", m_Ip, rcv, avg);

	return true;
}
;

void CIp::MultiplePing() {

	fclose(m_fPingSrc);

	if (system(
			"/usr/sbin/fping -a </tmp/fpingsrc.txt 2>/dev/null 1>/tmp/fpingres.txt")
			== -1)
		throw CErrorCommon(FEEXECFPING);

	m_fPingSrc = fopen("/tmp/fpingsrc.txt", "w");
	m_fPingRes = fopen("/tmp/fpingres.txt", "r");

	if (!m_fPingSrc || !m_fPingRes)
		throw CErrorCommon(FEOPENFPINGSRCORRES);

}
;

void CIp::MultiplePingAddHost(const char *pIp) {

	if (m_fPingSrc)
		fputs(pIp, m_fPingSrc);

}
;

bool CIp::MultiplePingGetHost(char *pIp) {
	if (m_fPingRes)
		if (!feof(m_fPingRes)) {
			fgets(pIp, 16, m_fPingRes);
			strtok(pIp, "\n");
			return true;
		} else {
			fclose(m_fPingRes);
			m_fPingRes = fopen("/tmp/fpingres.txt", "r");
			if (!m_fPingRes)
				throw CErrorCommon(FEOPENFPINGRES);
		};
	return false;
}
;

bool CIp::SnmpGet(const std::string& snmp, std::string& snmpResult) {

	std::string rawResult;
	vector<std::string> tokensResult;

/*
	cout << __FILE__ << ":" << __FUNCTION__ << " pthread_mutex_lock" << endl;
	//critical section
	pthread_mutex_lock(&CIp::mutex);
	InitSnmpSession();
	pthread_mutex_unlock(&CIp::mutex);
	SnmpGetRaw(snmp, m_sess_handle, rawResult);
	pthread_mutex_lock(&CIp::mutex);
	CloseSnmpSession();
	pthread_mutex_unlock(&CIp::mutex);
	//end of critical section
	cout << __FILE__ << ":" << __FUNCTION__ << " pthread_mutex_unlock" << endl;
*/

	try {
		InitSnmpSession();
		SnmpGetRaw(snmp, m_sess_handle, rawResult);
		CloseSnmpSession();
	} catch (CErrorCommon& e) {
		cout << __FILE__ << ":" << __FUNCTION__ << " Error: " << e.getMessage() << endl;
	}

	snmpResult.clear();
	tokensResult.clear();

 	CStringUtil::tokenizeFindSubstring(rawResult, ": ", tokensResult);

	if (tokensResult.size() >= 2) {
		for(int i = 1; i < tokensResult.size(); i++) {
			snmpResult += tokensResult[i];
		}
	}
}

/**
 * Snmp Get Raw result.
 * Wrap snmp get net-snmp functionality.
 */
bool CIp::SnmpGetRaw(const std::string& snmp, struct snmp_session* sess_handle,
		std::string& rawResult) {

	struct snmp_pdu *pdu = NULL;
	struct snmp_pdu *response = NULL;
	struct variable_list *vars;
	oid id_oid[MAX_OID_LEN];
	size_t id_len = MAX_OID_LEN;
	int status;
	char buff[8192];

	pdu = snmp_pdu_create(SNMP_MSG_GET);

	if (!read_objid(snmp.c_str(), id_oid, &id_len) || !pdu) {
		std::stringstream error;
		error << __FUNCTION__ << ":" << __LINE__ << ": ";
		error << "read_objid error: " << m_Ip << ": " << snmp;
		throw CErrorCommon(error.str());
	}

	if (pdu != NULL) {
		snmp_add_null_var(pdu, id_oid, id_len);
	} else {
		std::stringstream error;
		error << __FUNCTION__ << ":" << __LINE__ << ": ";
		error << " pdu: " << pdu;
		error << " snmp_add_null_var error: " << m_Ip << ": " << snmp;
		throw CErrorCommon(error.str());
	}

	if (pdu != NULL && sess_handle != NULL) {
		status = snmp_synch_response(sess_handle, pdu, &response);
	} else {
		std::stringstream error;
		error << __FUNCTION__ << ":" << __LINE__ << ": ";
		error << " pdu: " << pdu << " sess_handle: " << sess_handle;
		error << " snmp_synch_response error: " << m_Ip << ": " << snmp;
		throw CErrorCommon(error.str());
	}

	if (status == STAT_SUCCESS) {
		if (response->errstat == SNMP_ERR_NOERROR) {
			vars = response->variables;
			snprint_value(buff, sizeof(buff) - 1, vars->name, vars->name_length,
					vars);

			rawResult = buff;

			if (response != NULL) {
				snmp_free_pdu(response);
			} else {
				std::stringstream error;
				error << __FUNCTION__ << ":" << __LINE__ << ": ";
				error << " pdu: " << pdu << " response: " << response;
				error << " snmp_free_pdu error: " << m_Ip << ": " << snmp;
				throw CErrorCommon(error.str());
			}

			return true;
		}
	}

	snmp_free_pdu(response);

	return false;
}

/**
 * Snmp GET
 * TODO: Consider Rework - very ugly C-styled snmp result parse. Better make it like snmp walk.
 */
bool CIp::SnmpGet(const char *snmp, struct snmp_session* sess_handle,
		char* snmpResult) {

	struct snmp_pdu *pdu, *response;
	struct variable_list *vars;
	oid id_oid[MAX_OID_LEN];
	size_t id_len = MAX_OID_LEN;
	int status;
	char buff[1024];

	pdu = snmp_pdu_create(SNMP_MSG_GET);

	if (!read_objid(snmp, id_oid, &id_len)) {
		sprintf(buff, "%s - %s(%s)", FEREADSNMP, m_Ip, snmp);
		throw CErrorCommon(buff);
	}

	snmp_add_null_var(pdu, id_oid, id_len);

	status = snmp_synch_response(sess_handle, pdu, &response);

	if (status == STAT_SUCCESS) {
		if (response->errstat == SNMP_ERR_NOERROR) {
			vars = response->variables;
			snprint_value(buff, sizeof(buff) - 1, vars->name, vars->name_length,
					vars);

			// Copy SnmpResult to main proccess
			if (strncmp(buff, "STRING", 6) == 0) {
				buff[strlen(buff) - 1] = 0;
				strcpy(snmpResult, strchr(buff, '"') + 1);
			} else
				strcpy(snmpResult, strchr(buff, ' ') + 1);

			snmp_free_pdu(response);

			return true;
		}
	}

	snmp_free_pdu(response);

	return false;
}

bool CIp::SnmpGet(const std::string& snmp, char* snmpResult) {
	SnmpGet(snmp.c_str(), m_sess_handle, snmpResult);
}

bool CIp::SnmpGet(const std::string& snmp, const struct snmp_session* sess_handle,
		char* snmpResult) {
	SnmpGet(snmp.c_str(), sess_handle, snmpResult);
}

bool CIp::SnmpGet(const char *snmp, const char *community, char *snmpResult) {

	struct snmp_session* sess_handle;

	InitSnmpSession(community, &sess_handle);

	SnmpGet(snmp, sess_handle, snmpResult);

	CloseSnmpSession(sess_handle);

	return false;

	/*  char command[100];
	 FILE * shellPipe;

	 sprintf(command, "snmpget -c %s %s %s -Ovq 2>/dev/null || echo ''", community, m_Ip, snmp);
	 if ( (shellPipe = popen(command, "r"))==NULL ) throw CErrorCommon( FEEXECSNMP );
	 if ( fread(snmpResult, sizeof(char), 200, shellPipe)==1 ) return false;
	 strtok( snmpResult,"\n" );

	 return true; */

}
;

/**
 * Make multiple Snmp Get using one session handler (avoid multiple session open/close)
 */
bool CIp::SnmpGetMultiple(const vector<std::string>& snmpOid, const char *community,
		vector<std::string>* snmpResultList) {

	char snmpResult[256] = { 0 };
	struct snmp_session* sess_handle;

	snmpResultList->clear();

	InitSnmpSession(community, &sess_handle);

	for (int i = 0; i < snmpOid.size(); i++) {
		SnmpGet(snmpOid[i], sess_handle, snmpResult);
		snmpResultList->push_back(snmpResult);
	}

	CloseSnmpSession(sess_handle);

	return true;
}

bool CIp::SnmpSet(const char *snmp, const char *community, char snmpType,
		char *snmpValue, char *snmpResult) {
	struct snmp_session* sess_handle;

	struct snmp_pdu *pdu, *response;
	struct variable_list *vars;
	oid id_oid[MAX_OID_LEN];
	size_t id_len = MAX_OID_LEN;
	int status;

	InitSnmpSession(community, &sess_handle);

	pdu = snmp_pdu_create(SNMP_MSG_SET);

	read_objid(snmp, id_oid, &id_len);

	snmp_add_var(pdu, id_oid, id_len, snmpType, snmpValue);
	//snmp_add_null_var(pdu, id_oid, id_len);

	status = snmp_synch_response(sess_handle, pdu, &response);

	if (status == STAT_SUCCESS) {
		if (response->errstat == SNMP_ERR_NOERROR) {
			vars = response->variables;
			snprint_value(m_SnmpResult, 100, vars->name, vars->name_length,
					vars);
			strcpy(snmpResult, strchr(m_SnmpResult, ' ') + 1);

			snmp_free_pdu(response);
			snmp_close(sess_handle);

			return true;
		}
	}

	snmp_free_pdu(response);

	CloseSnmpSession(sess_handle);

	return false;

	/*  char command[100];
	 FILE * shellPipe;

	 sprintf(command, "snmpget -c %s %s %s -Ovq 2>/dev/null || echo ''", community, m_Ip, snmp);
	 if ( (shellPipe = popen(command, "r"))==NULL ) throw CErrorCommon( FEEXECSNMP );
	 if ( fread(snmpResult, sizeof(char), 200, shellPipe)==1 ) return false;
	 strtok( snmpResult,"\n" );

	 return true; */

}
;

bool CIp::SnmpWalk(const char *snmp, const char *community,
		vector<std::string> *snmpOID, vector<std::string> *snmpResult) {

	struct snmp_session* sess_handle;

	struct snmp_pdu *pdu, *response;
	struct variable_list *vars;
	oid id_oid[MAX_OID_LEN], root[MAX_OID_LEN];
	size_t id_len = MAX_OID_LEN, rootlen = MAX_OID_LEN;
	int status=0, getnext=0;
	char buff[200];

	InitSnmpSession(community, &sess_handle);

	if (!read_objid(snmp, id_oid, &id_len) || !read_objid(snmp, root, &rootlen)) {
		sprintf(buff, "%s - %s(%s)", FEREADSNMP, m_Ip, snmp);
		throw CErrorCommon(buff);
	}

	while (getnext) {
		getnext = 0;
		pdu = snmp_pdu_create(SNMP_MSG_GETNEXT);

		/*    if ( !read_objid(snmp, id_oid, &id_len) ) {
		 sprintf( buff, "%s - %s(%s)", FEREADSNMP, m_Ip, snmp );
		 throw CErrorCommon( buff );
		 }
		 */

		snmp_add_null_var(pdu, id_oid, id_len);
		status = snmp_synch_response(sess_handle, pdu, &response);

		if (status == STAT_SUCCESS) {
			if (response->errstat == SNMP_ERR_NOERROR) {

				for (vars = response->variables; vars; vars
						= vars->next_variable) {
					if (vars->name_length < rootlen || memcmp(root, vars->name,
							rootlen * sizeof(oid)))
						/* not part of this subtree */
						continue;

					// Copy SnmpResult to main proccess
					//print_variable(vars->name, vars->name_length, vars);
					//snprint_value( m_SnmpResult, 100, vars->name, vars->name_length, vars );
					snprint_variable(buff, 200, vars->name, vars->name_length,
							vars);
					buff[strcmp(buff, strchr(buff, '='))] = 0;
					strcpy(m_SnmpOID, strrchr(buff, '.') + 1);
					strchr(m_SnmpOID, ' ')[0] = 0;

					snprint_value(buff, 100, vars->name, vars->name_length,
							vars);
					if (strncmp(buff, "STRING", 6) == 0) {
						buff[strlen(buff) - 1] = 0;
						strcpy(m_SnmpResult, strchr(buff, '"') + 1);
					} else
						strcpy(m_SnmpResult, strchr(buff, ' ') + 1);

					snmpOID->push_back(m_SnmpOID);
					snmpResult->push_back(m_SnmpResult);
					if (vars->type != SNMP_ENDOFMIBVIEW && vars->type
							!= SNMP_NOSUCHOBJECT && vars->type
							!= SNMP_NOSUCHINSTANCE) {
						memmove((char *) id_oid, (char *) vars->name,
								vars->name_length * sizeof(oid));
						id_len = vars->name_length;
						getnext = 1;
					}
				}
			} else {
				if (response)
					snmp_free_pdu(response);

				CloseSnmpSession(sess_handle);

				return false;
			}
		}
	}

	if (response)
		snmp_free_pdu(response);

	CloseSnmpSession(sess_handle);

	return true;
}
;

bool CIp::SnmpWalkBulk(const std::string& snmp, vector<std::string>* snmpOID, vector<std::string>* snmpResult) {

	SnmpWalkBulk(snmp.c_str(), m_sess_handle, snmpOID, snmpResult);

	return true;
}
bool CIp::SnmpWalkBulk(const std::string& snmp, const std::string& community, vector<std::string>* snmpOID, vector<std::string>* snmpResult) {

	InitSnmpSession(community);

	SnmpWalkBulk(snmp.c_str(), m_sess_handle, snmpOID, snmpResult);

	CloseSnmpSession();

	return true;
}

/**
 * Snmp Bulk Walk - much faster than normal Snmp Walk.
 * TODO: Change pointer to vector (pass by reference)
 *
 */
bool CIp::SnmpWalkBulk(	const char* snmp, netsnmp_session* sess_handle,
						vector<std::string>* snmpOID, vector<std::string> *snmpResult) {

	vector<std::string> rawResult;

	std::string sid;
	std::string snmpValue;

	unsigned int i;

	SnmpWalkBulkRaw(snmp, sess_handle, rawResult);

	for(i = 0; i < rawResult.size(); i++) {

		parseSnmpResult(rawResult[i], sid, snmpValue);

		snmpOID->push_back(sid);
		snmpResult->push_back(snmpValue);
	}

	return true;
}

/**
 * Snmp Bulk Walk. Return Raw result set.
 *
 */
bool CIp::SnmpWalkBulkRaw(	const char* snmp,
							netsnmp_session* sess_handle,
							vector<std::string>& rawResult) {

	netsnmp_pdu *pdu, *response;
	netsnmp_variable_list *vars;

	oid id_oid[MAX_OID_LEN];
	oid root[MAX_OID_LEN];
	oid name[MAX_OID_LEN];
	size_t id_len = MAX_OID_LEN;
	size_t rootlen = MAX_OID_LEN;
	size_t name_length = MAX_OID_LEN;

	int status;
	int running;
	int exitval = 0;
	int numRetries = 10;

	char buff[4096];

	if (!read_objid(snmp, id_oid, &id_len) || !read_objid(snmp, root, &rootlen)) {
		std::stringstream error;
		error << __FUNCTION__ << ":" << __LINE__ << ": ";
		error << "read_objid error: " << m_Ip << ": " << snmp;
		throw CErrorCommon(error.str());
	}

	rawResult.clear();

	running = 1;

	while (running) {
		/*
		 * create PDU for GETBULK request and add object name to request
		 */
		pdu = snmp_pdu_create(SNMP_MSG_GETBULK);
		pdu->non_repeaters = 0;
		pdu->max_repetitions = 65536;

		snmp_add_null_var(pdu, id_oid, id_len);

		status = snmp_synch_response(sess_handle, pdu, &response);

		if (status == STAT_SUCCESS) {
			if (response->errstat == SNMP_ERR_NOERROR) {
				/*
				 * check resulting variables
				 */
				for (vars = response->variables; vars; vars
						= vars->next_variable) {
					if ((vars->name_length < rootlen) || (memcmp(root,
							vars->name, rootlen * sizeof(oid)) != 0)) {
						/*
						 * not part of this subtree
						 */
						running = 0;
						continue;
					}

					snprint_variable(buff, sizeof(buff), vars->name, vars->name_length,
							vars);

					rawResult.push_back(buff);

					if ((vars->type != SNMP_ENDOFMIBVIEW) && (vars->type
							!= SNMP_NOSUCHOBJECT) && (vars->type
							!= SNMP_NOSUCHINSTANCE)) {
						/*
						 * not an exception value
						 */
						if (snmp_oid_compare(id_oid, id_len, vars->name,
								vars->name_length) >= 0) {

							std::stringstream error;
							error << __FUNCTION__ << ":" << __LINE__ << ": ";
							error << "OID not increasing: " << m_Ip << ": " << snmp;
							cout << __FUNCTION__ << ":pid:" << getpid() << " ERROR: " << error.str() << endl;
							//throw CErrorCommon(error.str());
							return false;
							/*
							running = 0;
							exitval = 1;
							*/
						}

						/*
						 * Check if last variable, and if so, save for next request.
						 */
						if (vars->next_variable == NULL) {
							memmove(id_oid, vars->name,
									vars->name_length * sizeof(oid));
							id_len = vars->name_length;
						}
					} else {
						/*
						 * an exception value, so stop
						 */
						running = 0;
					}
				}
			} else {
				/*
				 * error in response, print it
				 */
				running = 0;
				if (response->errstat == SNMP_ERR_NOSUCHNAME) {
					printf("End of MIB\n");
				} else {
					std::stringstream error;
					error <<  __FUNCTION__ << ":" << __LINE__ << ": ";
					error << "Reason: " << snmp_errstring(response->errstat);
					throw CErrorCommon(error.str());
					/*
					fprintf(stderr, "Error in packet.\nReason: %s\n",
							snmp_errstring(response->errstat));
					if (response->errindex != 0) {
						fprintf(stderr, "Failed object: ");
						fprintf(stderr, "\n");
					}
					exitval = 2;
					*/
				}
			}
		} else if (status == STAT_TIMEOUT) {

			numRetries--;
			if (numRetries > 0) {
				//cout << "Timeout sleeping... " << numRetries << endl;
				sleep(3);
			} else {
			std::stringstream error;
			error <<  __FUNCTION__ << ":" << __LINE__;
			error << ":STAT_TIMEOUT:";
			error << snmp << ":" << m_Ip;

			throw CErrorCommon(error.str());
			}
		} else { /* status == STAT_ERROR */
			std::stringstream error;
			error << __FUNCTION__ << ":" << __LINE__;
			error << ":STAT_ERROR";
			error << snmp << ":" << m_Ip;

			throw CErrorCommon(error.str());
		}

		if (response)
			snmp_free_pdu(response);
	}

	return true;
}

/**
 * Read Snmp and get last 2 Indexes and the snmp value.
 *
 */
bool CIp::SnmpWalkBulkIdx2(const std::string& snmp, CDataSet& result) {

	vector<std::string> rawResult;
	vector<std::string> row;

	unsigned int i;

	InitSnmpSession();

	SnmpWalkBulkRaw(snmp.c_str(), m_sess_handle, rawResult);

	CloseSnmpSession();

	result.clear();

	result.addColumn(KEY_INDEX"0");
	result.addColumn(KEY_INDEX"1");
	result.addColumn(KEY_VALUE);

	for(i = 0; i < rawResult.size(); i++) {

		row.clear();

		vector<std::string> tokenDots;
		std::string snmpValue;

		// Parse Raw Snmp Result and get last 2 indexes and the value
		if (parseSnmpResult(rawResult[i], tokenDots, snmpValue)) {

			if (tokenDots.size() >= 2) {

				row.push_back(tokenDots[tokenDots.size() - 1]);
				row.push_back(tokenDots[tokenDots.size() - 2]);
				row.push_back(snmpValue);

				result.addRow(row);

			}

		} else {
			cout << __FUNCTION__ << ":" << __LINE__ << ":ERROR:pid:" << getpid();
			cout << " rawSnmpResult[" << rawResult[i] << "]" << endl;
		}

	}

	return true;
}

/**
 * Read DOCSIS3 formated snmp walk result.
 * format is like oid.sid.if_id = INTEGER: 345
 * (SNMPv2-SMI::enterprises.4491.2.1.20.1.4.1.4.2476232.3989 = INTEGER: 361)
 *
 */
bool CIp::SnmpWalkBulkDocs3(const std::string& snmp, const std::string& community, CDataSet& result) {

	vector<std::string> rawResult;
	vector<std::string> row;

	unsigned int i;

	InitSnmpSession(community);

	SnmpWalkBulkRaw(snmp.c_str(), m_sess_handle, rawResult);

	CloseSnmpSession();

	result.clear();

	result.addColumn(KEY_SID);
	result.addColumn(KEY_IF_ID);
	result.addColumn(KEY_VALUE);

	for(i = 0; i < rawResult.size(); i++) {

		row.clear();

		if (parseSnmpResultDocs3(rawResult[i], row)) {

			result.addRow(row);

		} else {
			cout << __FUNCTION__ << ":" << __LINE__ << ":ERROR:pid:" << getpid() << endl;
		}

	}

	return true;
}

/**
 * Read SNMP Indexed by MAC (MAC is converted into 6 decimal numbers
 * separated with dots and added after main snmp oid part.
 *
 */
bool CIp::SnmpWalkBulkIndexedByMAC(const std::string& snmp, const std::string& community, CDataSet& result) {

	vector<std::string> rawResult;
	vector<std::string> row;

	unsigned int i;

	InitSnmpSession(community);

	SnmpWalkBulkRaw(snmp.c_str(), m_sess_handle, rawResult);

	CloseSnmpSession();

	result.clear();

	result.addColumn(KEY_MAC_HEX);
	result.addColumn(KEY_MAC_DEC);
	result.addColumn(KEY_MAC_DEC_IDX);
	result.addColumn(KEY_VALUE);

	for(i = 0; i < rawResult.size(); i++) {

		row.clear();

		if (parseSnmpResultIndexedByMac(rawResult[i], row)) {

			result.addRow(row);

		} else {
			cout << __FUNCTION__ << ":" << __LINE__ << ":ERROR:pid:" << getpid() << endl;
		}

	}

	return true;
}


/**
 * Parse Raw DOCSIS3 Snmp Result.
 * Should contain SID, IF_ID, SnmpValue added to a DataRow.
 */
bool CIp::parseSnmpResultDocs3(const std::string& rawResult, vector<std::string>& row) {

	std::string cmSid;
	std::string cmIfId;
	std::string snmpValue;

	vector<std::string> tokensEqual;
	vector<std::string> tokensDots;
	vector<std::string> tokensColon;

	CErrorCommon cerr("err");

	CStringUtil::tokenize(rawResult, "=", tokensEqual);

	if (tokensEqual.size() >= 2) {

		CStringUtil::tokenize(tokensEqual[0], ".", tokensDots);

		CStringUtil::tokenize(tokensEqual[1], ":", tokensColon);

		if (tokensColon.size() >= 2) {

			snmpValue = tokensColon[1];

			snmpValue.erase(0, 1);

		} else {
			cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid();
			cout << " ERROR: tokensColon.size:" << tokensColon.size() << endl;
			return false;
		}

		if (tokensDots.size() >= 2) {

			cmIfId = tokensDots[tokensDots.size() - 1];

			cmSid = tokensDots[tokensDots.size() - 2];

			CStringUtil::remove(' ', cmIfId);

		} else {
			cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid();
			cout << " ERROR: tokensDots.size:" << tokensDots.size() << endl;
			return false;
		}

		row.push_back(cmSid);
		row.push_back(cmIfId);
		row.push_back(snmpValue);

	} else {
		cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid();
		cout << " ERROR: tokensEqual.size:" << tokensEqual.size() << endl;
		return false;
	}

	return true;
}

/**
 * Parse Raw Snmp Result for a MAC indexed SNMP OIDs.
 * Should contain MAC, SnmpValue added to a DataRow.
 */
bool CIp::parseSnmpResultIndexedByMac(const std::string& rawResult, vector<std::string>& row) {

	std::string cmMac;
	std::string snmpValue;

	vector<std::string> tokensDots;

	CErrorCommon cerr("err");

	if (parseSnmpResult(rawResult, tokensDots, snmpValue)) {

		if (tokensDots.size() >= 6) {

			char mac[32] = { 0 };
			char macNoColons[32] = { 0 };
			std::string hexMac;
			std::string decMac;
			std::string rawMac;

			sprintf(mac, "%2x:%2x:%2x:%2x:%2x:%2x",
					atoi(tokensDots[tokensDots.size() - 6].c_str()),
					atoi(tokensDots[tokensDots.size() - 5].c_str()),
					atoi(tokensDots[tokensDots.size() - 4].c_str()),
					atoi(tokensDots[tokensDots.size() - 3].c_str()),
					atoi(tokensDots[tokensDots.size() - 2].c_str()),
					atoi(tokensDots[tokensDots.size() - 1].c_str()));

			sprintf(macNoColons, "%2x%2x%2x%2x%2x%2x",
					atoi(tokensDots[tokensDots.size() - 6].c_str()),
					atoi(tokensDots[tokensDots.size() - 5].c_str()),
					atoi(tokensDots[tokensDots.size() - 4].c_str()),
					atoi(tokensDots[tokensDots.size() - 3].c_str()),
					atoi(tokensDots[tokensDots.size() - 2].c_str()),
					atoi(tokensDots[tokensDots.size() - 1].c_str()));

			hexMac = macNoColons;
			CStringUtil::replace(hexMac, ' ', '0');
			CStringUtil::convertMac2Dec(hexMac, decMac);

			CStringUtil::implode(tokensDots.size() - 6, tokensDots.size(),
					tokensDots, ".", rawMac);

			hexMac = mac;
			CStringUtil::replace(hexMac, ' ', '0');

			row.push_back(hexMac);
			row.push_back(decMac);
			row.push_back(rawMac);
			row.push_back(snmpValue);

		} else {
			cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid();
			cout << " ERROR: tokensDots.size:" << tokensDots.size() << endl;
			return false;
		}

	} else {
		cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid();
		cout << " ERROR: parseSnmpResult" << endl;
		return false;
	}

	return true;
}

/**
 * Parse Raw Snmp Result - tokenize oid chain and fill snmp value.
 */
bool CIp::parseSnmpResult(const std::string& rawResult, vector<std::string>& tokensDots, std::string& snmpValue) {

	vector<std::string> tokensEqual;
	vector<std::string> tokensColon;

	CErrorCommon cerr("err");

	CStringUtil::tokenize(rawResult, "=", tokensEqual);

	if (tokensEqual.size() >= 2) {

		CStringUtil::tokenize(tokensEqual[0], ".", tokensDots);

		CStringUtil::tokenize(tokensEqual[1], ":", tokensColon);

		if (tokensColon.size() >= 2) {

			snmpValue = tokensColon[1];

			snmpValue.erase(0, 1);

		} else {
			cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid();
			cout << " ERROR: tokensColon.size:" << tokensColon.size() << endl;
			return false;
		}


	} else {
		cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid();
		cout << " ERROR: tokensEqual.size:" << tokensEqual.size() << endl;
		return false;
	}

	if (tokensDots.size() > 0) {
		std::string tokenLast = tokensDots[tokensDots.size() - 1];
		CStringUtil::remove(' ', tokenLast);
		tokensDots[tokensDots.size() - 1] = tokenLast;
	}

	return true;
}

bool CIp::parseSnmpResult(const std::string& rawResult, std::string& idx, std::string& snmpValue) {

	vector<std::string> tokensDots;

	parseSnmpResult(rawResult, tokensDots, snmpValue);

	if(tokensDots.size() > 0) {
		idx = tokensDots[tokensDots.size() - 1];
		return true;
	} else {
		return false;
	}
}

/**
 * Parse buffer with snmp data like
 * "iso.3.6.1.2.1.10.127.1.3.3.1.4.852217 = INTEGER: 209715296"
 * Should get result and sid from the buffer (for ex. sid=852217 and result=209715296)
 * TODO: Consider not using c-style parsing...
 *
 */
bool CIp::parseSnmpResultOld(const char* buff, char* sid, char* snmpResult) {

	char* cp1;
	char* cp2;
	char* cp3;

	cp1 = strstr(const_cast<char*> (buff), " = ");
	cp2 = strstr(const_cast<char*> (buff), ": ");

	// Found =
	if (cp1) {
		for(cp3 = cp1; cp3 > buff && *cp3 != '.'; cp3--);

		// Found sid
		if (cp3 > buff && cp1 && cp1 > cp3) {
			cp3++;
			strncpy(sid, cp3, cp1 - cp3);
			sid[cp1 - cp3] = 0;
		}

	}

	if (cp2) {
		strcpy(snmpResult, cp2 + 2);
	}

	//cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid();
	//cout << " BUF:[" << buff << "] SID:[" << sid << "] res:[" << snmpResult << "]" << endl;

	return true;
}

/**
 * Set Snmp Community
 */
void CIp::setCommunity(const std::string& community) {
	m_community = community;
}

bool CIp::InitSnmpSession() {
	return InitSnmpSession(m_community);
}

bool CIp::InitSnmpSession(const std::string& community) {
	return InitSnmpSession(community.c_str(), &m_sess_handle);
}

bool CIp::InitSnmpSession(const char *community) {
	return InitSnmpSession(community, &m_sess_handle);
}

/**
 * Initialize Snmp Session and make a Session Handler.
 */
bool CIp::InitSnmpSession(const char *community,
		struct snmp_session** sess_handle) {

	struct snmp_session session;

	snmp_sess_init(&session);
	session.version = SNMP_VERSION_2c;
	session.community = (u_char*) community;
	session.community_len = strlen((char*) session.community);
	session.peername = m_Ip;

	SOCK_STARTUP;

	*sess_handle = snmp_open(&session);

	if (!*sess_handle) {
		throw CErrorCommon(__FUNCTION__);
	}

	return true;
}

/**
 * Close Snmp Session
 */
bool CIp::CloseSnmpSession() {

	CloseSnmpSession(m_sess_handle);

	return true;
}

/**
 * Close Snmp Session
 */
bool CIp::CloseSnmpSession(struct snmp_session* sess_handle) {

	snmp_close(sess_handle);

	SOCK_CLEANUP;

	return true;
}

bool CIp::Alive() {
	unsigned int rcv, avg;

	Ping(rcv, avg, 54);
	if (rcv > 0)
		return false;
	else
		return true;
}
;

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
