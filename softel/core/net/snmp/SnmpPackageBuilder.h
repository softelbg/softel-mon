/*
 * SnmpPackageBuilder.h
 *
 *  Created on: 20.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef SNMPPACKAGEBUILDER_H_
#define SNMPPACKAGEBUILDER_H_

#include <vector>
#include <string>
#include <softel/core/CObject.h>
#include "SnmpSession.h"
#include <softel/core/util/DateTime.h>


namespace SF {
namespace CORE {
namespace NET {

/**
 *---------------------------------------------------------------------
 *
 * class SnmpPackageBuilder --
 *
 *    Build the SNMP trap package which will be sent through UDP/IP.
 *
 *    The package is built from rear to front in order to make it
 *    efficient.
 *
 *---------------------------------------------------------------------
 */
class SnmpPackageBuilder: public SF::CORE::CObject {
public:
	SnmpPackageBuilder();
	virtual ~SnmpPackageBuilder();

	const unsigned char *GetBuffer() const {
		return m_buffer + m_available;
	}
	int GetSize() const {
		return sizeof(m_buffer) - m_available;
	}

	bool PushOID(int type, const std::string& oid);
	bool PushIP(int type, const std::string& ip);
	bool PushString(int type, const std::string& str);
	bool PushUnLong(int type, unsigned long value);
	bool PushLong(int type, long value);
	bool PushSequence(int type, int length);
	bool PushDateTime(int type, const SF::CORE::UTIL::DateTime& dateTime);

	static bool ValidateOID(const std::string& oid);
	static bool SplitDotString(const std::string& oid, std::vector<int>& nodes);

private:
	bool PushHeader(int type, int length);
	bool PushLength(int length);
	bool PushByte(unsigned char ch);
	bool PushBlock(const char *buf, int length);
	bool PushUnsignedBlock(const unsigned char *buf, int length);

	unsigned char m_buffer[SNMP_BUFFER_SIZE];
	int m_available;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* SNMPPACKAGEBUILDER_H_ */
