#ifndef CIp_H
#define CIp_H

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include <softel/core/inc/ErrorMsg.h>
#include <softel/core/error/CErrorCommon.h>
#include <softel/db/CDataSet.h>
#include <softel/db/CDataRow.h>

namespace SF {
namespace CORE {
namespace NET {

#define KEY_SID			"sid"
#define KEY_IF_ID		"ifid"
#define KEY_MAC_HEX		"mac_hex"
#define KEY_MAC_DEC		"mac_dec"
#define KEY_MAC_DEC_IDX	"mac_dec_idx"
#define KEY_VALUE		"value"
#define KEY_INDEX		"index"

class CIp {
public:

	CIp();
	CIp(const char *pIp);
	CIp(const std::string& ip);
	CIp(const std::string& ip, const std::string& community);

	~CIp();

	bool Ping(unsigned int &rcv, unsigned int &avg, unsigned int PacketSize);
	void MultiplePing();
	void MultiplePingAddHost(const char *pIp);
	bool MultiplePingGetHost(char *pIp);

	bool Alive();

	bool SnmpGetRaw(const std::string& snmp, struct snmp_session* sess_handle, std::string& rawResult);
	bool SnmpGet(const std::string& snmp, std::string& snmpResult);

	bool SnmpGet(const std::string& snmp, char* snmpResult);
	bool SnmpGet(const std::string& snmp, const struct snmp_session* sess_handle,
			char* snmpResult);
	bool SnmpGet(const char *snmp, struct snmp_session* sess_handle,
			char* snmpResult);
	bool SnmpGet(const char *snmp, const char *community, char *snmpResult);

	bool SnmpGetMultiple(const std::vector<std::string>& snmpOid, const char *community,
			std::vector<std::string>* snmpResult);

	bool SnmpSet(const char *snmp, const char *community, char snmpType,
			char *snmpValue, char *snmpResult);

	bool SnmpWalk(const char *snmp, const char *community,
			std::vector<std::string> *snmpOID, std::vector<std::string> *snmpResult);

	bool SnmpWalkBulk(const std::string& snmp, std::vector<std::string>* snmpOID,
			std::vector<std::string>* snmpResult);
	bool SnmpWalkBulk(const std::string& snmp, const std::string& community,
			std::vector<std::string>* snmpOID, std::vector<std::string>* snmpResult);
	bool SnmpWalkBulk(const char* snmp, netsnmp_session* sess_handle,
			std::vector<std::string>* snmpOID, std::vector<std::string>* snmpResult);

	// Raw Bulk Walk
	bool SnmpWalkBulkRaw(	const char* snmp,
							netsnmp_session* sess_handle,
							std::vector<std::string>& rawResult);

	bool SnmpWalkBulkIdx2(const std::string& snmp, SF::DB::CDataSet& result);
	bool SnmpWalkBulkDocs3(const std::string& snmp, const std::string& community, SF::DB::CDataSet& resultSet);
	bool SnmpWalkBulkIndexedByMAC(const std::string& snmp, const std::string& community, SF::DB::CDataSet& result);

	bool InitSnmpSession(const char *community,
			struct snmp_session** sess_handle);
	bool InitSnmpSession();
	bool InitSnmpSession(const char *community);
	bool InitSnmpSession(const std::string& community);
	bool CloseSnmpSession();
	bool CloseSnmpSession(struct snmp_session* sess_handle);

	void setIp(const std::string& ip);
	void setIp(const char *pIp);
	void setCommunity(const std::string& community);

	static pthread_mutex_t mutex;

private:

	bool parseSnmpResultOld(const char* buff, char* sid, char* snmpResult);

	bool parseSnmpResult(const std::string& rawResult, std::string& idx, std::string& value);
	bool parseSnmpResult(const std::string& rawResult, std::vector<std::string>& tokensOid, std::string& value);

	bool parseSnmpResultDocs3(const std::string& rawResult, std::vector<std::string>& row);

	bool parseSnmpResultIndexedByMac(const std::string& rawResult, std::vector<std::string>& row);


	std::string m_community;

	// TODO: Consider remove these C-style...
	char m_Ip[17];
	char m_SnmpOID[51];
	char m_SnmpResult[101];
	struct snmp_session* m_sess_handle;

	FILE *m_fPingSrc, *m_fPingRes;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

#endif
