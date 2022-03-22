/**
 * Snmp Value
 *
 *	@author sgeorgiev
 *
 *		www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSnmpValue_H
#define CSnmpValue_H

#include <iostream>
#include <stdio.h>

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "CSnmpOid.h"
#include "SnmpOids.h"

namespace SF {
namespace OSS {
namespace CMTS {

class CSnmpValue
{
public:

	CSnmpValue();
	CSnmpValue(const CSnmpOid& snmpOid);
	CSnmpValue(const CSnmpOid& snmpOid, const std::string& snmpValue);
	virtual ~CSnmpValue();

	virtual void setSnmpValue(const std::string& val);
	virtual void setSnmpValueParsed(const std::string& val);
	virtual void setTimeStampUx();

	const CSnmpOid& getSnmpOid() const;

	virtual const std::string& getRawValue() const;
	virtual const std::string& getValue() const;
	virtual const long 	getValueInt() const;
	virtual const float 	getValueFloat() const;
	virtual long getTimeStampUx() const;

	bool isZero() const;

	void dumpStrInfo(std::stringstream& result) const; // Append Dump snmp value
	void dumpStrInfo(std::string& result) const; // Append Dump snmp value
	void dumpInfo();
	
	// XML representation of the snmp value
	std::string toXml() const;
	void toXml(std::stringstream& result) const;
	void toXml(std::string& result) const;
	void toFnPointXml(std::string& result) const;

protected:
	
	void parseSnmpValue();

	void parseStatusValue(); // Parse if the snmp oid is for the modem status

	CSnmpOid m_snmpOid;

	std::string	m_raw_value;

	std::string	m_value;
	long 	m_iValue;
	double	m_fValue;

	long	m_ux_time;

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif
