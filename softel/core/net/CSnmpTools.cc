/*
 * CSnmpTools.cc
 *
 *	Snmp tools
 *
 *  Created on: 11.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <softel/core/inc/ErrorMsg.h>
#include <softel/core/error/CErrorCommon.h>
#include "CSnmpTools.h"


namespace SF {
namespace CORE {
namespace NET {

CSnmpTools::CSnmpTools() {

}
CSnmpTools::CSnmpTools(const std::string& ip) {
	setIp(ip);
}

CSnmpTools::~CSnmpTools() {

}

void CSnmpTools::setIp(const std::string& ip) {
	m_ip = ip;
	strcpy(m_ip_c, m_ip.c_str());
}


bool CSnmpTools::SnmpGetAsynch(const char *snmp, const char *community, char *snmpResult) {

}

bool CSnmpTools::SnmpGet(const char *snmp, struct snmp_session* sess_handle,
		char* snmpResult) {

	struct snmp_pdu *pdu, *response;
	struct variable_list *vars;
	oid id_oid[MAX_OID_LEN];
	size_t id_len = MAX_OID_LEN;
	int status;

	char mSnmpResult[256] = {0};

	pdu = snmp_pdu_create(SNMP_MSG_GET);

	if (!read_objid(snmp, id_oid, &id_len)) {
		std::string errMsg = FEREADSNMP;
		errMsg += " : " + m_ip + " : " + snmp;
		throw CErrorCommon(errMsg);
	}

	snmp_add_null_var(pdu, id_oid, id_len);

	status = snmp_synch_response(sess_handle, pdu, &response);

	if (status == STAT_SUCCESS) {
		if (response->errstat == SNMP_ERR_NOERROR) {
			vars = response->variables;
			snprint_value(mSnmpResult, sizeof(mSnmpResult)-1, vars->name, vars->name_length,
					vars);

			// Copy SnmpResult to main proccess
			if (strncmp(mSnmpResult, "STRING", 6) == 0) {
				mSnmpResult[strlen(mSnmpResult) - 1] = 0;
				strcpy(snmpResult, strchr(mSnmpResult, '"') + 1);
			} else
				strcpy(snmpResult, strchr(mSnmpResult, ' ') + 1);

			snmp_free_pdu(response);

			return true;
		}
	}

	snmp_free_pdu(response);

	return false;
}

bool CSnmpTools::SnmpGet(const std::string& snmp, char* snmpResult) {
	SnmpGet(snmp.c_str(), m_sess_handle, snmpResult);
}

bool CSnmpTools::SnmpGet(const std::string& snmp, const struct snmp_session* sess_handle,
		char* snmpResult) {
	SnmpGet(snmp.c_str(), sess_handle, snmpResult);
}

bool CSnmpTools::SnmpGet(const char *snmp, const char *community, char *snmpResult) {

	struct snmp_session* sess_handle;

	InitSnmpSession(community, &sess_handle);

	SnmpGet(snmp, sess_handle, snmpResult);

	CloseSnmpSession(sess_handle);

	return false;
}
;

/**
 * Make multiple Snmp Get using one session handler (avoid multiple session open/close)
 */
bool CSnmpTools::SnmpGetMultiple(const vector<std::string>& snmpOid, const char *community,
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

bool CSnmpTools::SnmpWalk(const char *snmp, const char *community,
		vector<std::string> *snmpOID, vector<std::string> *snmpResult) {

	struct snmp_session* sess_handle;

	struct snmp_pdu *pdu, *response;
	struct variable_list *vars;
	oid id_oid[MAX_OID_LEN], root[MAX_OID_LEN];
	size_t id_len = MAX_OID_LEN, rootlen = MAX_OID_LEN;
	int status, getnext;
	char buff[256];


	InitSnmpSession(community, &sess_handle);

	if (!read_objid(snmp, id_oid, &id_len) || !read_objid(snmp, root, &rootlen)) {
		std::string errMsg = FEREADSNMP;
		errMsg += " : " + m_ip + " : " + snmp;
		throw CErrorCommon(errMsg);
	}

	while (getnext) {
		getnext = 0;
		pdu = snmp_pdu_create(SNMP_MSG_GETNEXT);

		/*    if ( !read_objid(snmp, id_oid, &id_len) ) {
			 	 std::string errMsg = FEREADSNMP;
				errMsg += " : " + m_ip + " : " + snmp;
				throw CErrorCommon(errMsg);
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
					//snprint_value( mSnmpResult, sizeof(mSnmpResult)-1, vars->name, vars->name_length, vars );
					snprint_variable(buff, sizeof(buff)-1, vars->name, vars->name_length,
							vars);
					buff[strcmp(buff, strchr(buff, '='))] = 0;
					strcpy(m_SnmpOID, strrchr(buff, '.') + 1);
					strchr(m_SnmpOID, ' ')[0] = 0;

					snprint_value(buff, 100, vars->name, vars->name_length,
							vars);
					if (strncmp(buff, "STRING", 6) == 0) {
						buff[strlen(buff) - 1] = 0;
						strcpy(mSnmpResult, strchr(buff, '"') + 1);
					} else
						strcpy(mSnmpResult, strchr(buff, ' ') + 1);

					snmpOID->push_back(m_SnmpOID);
					snmpResult->push_back(mSnmpResult);
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

bool CSnmpTools::SnmpWalkBulk(const std::string& snmp, vector<std::string>* snmpOID, vector<std::string>* snmpResult) {

	SnmpWalkBulk(snmp.c_str(), m_sess_handle, snmpOID, snmpResult);

	return true;
}
bool CSnmpTools::SnmpWalkBulk(const std::string& snmp, const std::string& community, vector<std::string>* snmpOID, vector<std::string>* snmpResult) {

	InitSnmpSession(community);

	SnmpWalkBulk(snmp.c_str(), m_sess_handle, snmpOID, snmpResult);

	CloseSnmpSession();

	return true;
}

/**
 * Snmp Bulk Walk - much faster than normal Snmp Walk.
 *
 */
bool CSnmpTools::SnmpWalkBulk(	const char* snmp, netsnmp_session* sess_handle,
						vector<std::string>* snmpOID, vector<std::string> *snmpResult) {

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

	char buff[256];

	if (!read_objid(snmp, id_oid, &id_len) || !read_objid(snmp, root, &rootlen)) {
		std::string errMsg = FEREADSNMP;
		errMsg += " : " + m_ip + " : " + snmp;
		throw CErrorCommon(errMsg);
	}

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

					snprint_variable(buff, 200, vars->name, vars->name_length,
							vars);
					buff[strcmp(buff, strchr(buff, '='))] = 0;
					strcpy(m_SnmpOID, strrchr(buff, '.') + 1);
					strchr(m_SnmpOID, ' ')[0] = 0;

					snprint_value(buff, 100, vars->name, vars->name_length,
							vars);
					if (strncmp(buff, "STRING", 6) == 0) {
						buff[strlen(buff) - 1] = 0;
						strcpy(mSnmpResult, strchr(buff, '"') + 1);
					} else
						strcpy(mSnmpResult, strchr(buff, ' ') + 1);

					snmpOID->push_back(m_SnmpOID);
					snmpResult->push_back(mSnmpResult);

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
							error << "OID not increasing: " << m_ip << ": " << snmp;
							throw CErrorCommon(error.str());
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
					error << "Reason: " << snmp_errstd::string(response->errstat);
					throw CErrorCommon(error.str());
					/*
					fprintf(stderr, "Error in packet.\nReason: %s\n",
							snmp_errstd::string(response->errstat));
					if (response->errindex != 0) {
						fprintf(stderr, "Failed object: ");
						fprintf(stderr, "\n");
					}
					exitval = 2;
					*/
				}
			}
		} else if (status == STAT_TIMEOUT) {
			std::stringstream error;
			error <<  __FUNCTION__ << ":" << __LINE__;
			error << ":STAT_TIMEOUT:";
			error << snmp << ":" << m_ip;

			throw CErrorCommon(error.str());
		} else { /* status == STAT_ERROR */
			std::stringstream error;
			error << __FUNCTION__ << ":" << __LINE__;
			error << ":STAT_ERROR";
			error << snmp << ":" << m_ip;

			throw CErrorCommon(error.str());
		}
		if (response)
			snmp_free_pdu(response);
	}

	return true;
}

bool CSnmpTools::InitSnmpSession(const std::string& community) {

	InitSnmpSession(community.c_str(), &m_sess_handle);

	return true;
}

bool CSnmpTools::InitSnmpSession(const char *community) {

	InitSnmpSession(community, &m_sess_handle);

	return true;
}

/**
 * Initialize Snmp Session and make a Session Handler.
 */
bool CSnmpTools::InitSnmpSession(const char *community,
		struct snmp_session** sess_handle) {

	struct snmp_session session;

	snmp_sess_init(&session);
	session.version = SNMP_VERSION_2c;
	session.community = (u_char*) community;
	session.community_len = strlen((char*) session.community);
	session.peername = m_ip_c;

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
bool CSnmpTools::CloseSnmpSession() {

	CloseSnmpSession(m_sess_handle);

	return true;
}

/**
 * Close Snmp Session
 */
bool CSnmpTools::CloseSnmpSession(struct snmp_session* sess_handle) {

	snmp_close(sess_handle);

	SOCK_CLEANUP;

	return true;
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

