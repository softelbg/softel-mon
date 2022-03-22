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

#include "CSnmpValue.h"

using namespace std;
using namespace SF::DB;
using namespace SF::CORE::UTIL;

namespace SF {
namespace OSS {
namespace CMTS {

CSnmpValue::CSnmpValue() {

}

CSnmpValue::CSnmpValue(const CSnmpOid& snmpOid) :
			m_snmpOid(snmpOid), m_ux_time(0) {

}

CSnmpValue::CSnmpValue(const CSnmpOid& snmpOid, const string& snmpValue) :
			m_snmpOid(snmpOid) {

	setSnmpValue(snmpValue);

}

CSnmpValue::~CSnmpValue() {

}

const CSnmpOid& CSnmpValue::getSnmpOid() const {
	return m_snmpOid;
}

void CSnmpValue::setSnmpValueParsed(const string& val) {
	m_raw_value = val;
	m_value = val;
	setTimeStampUx();
}

void CSnmpValue::setSnmpValue(const string& val) {
	m_raw_value = val;
	setTimeStampUx();
	parseSnmpValue();
}

long CSnmpValue::getTimeStampUx() const {
	return this->m_ux_time;
}

/**
 * Set current time stamp
 */
void CSnmpValue::setTimeStampUx() {
	//m_ux_time = SF::CORE::SYSTEM::CSystemTools::doGetCurrentTimeUx();
	m_ux_time = time(0); // resolve build problem for the system namespace
}

/**
 * Parse Status Value for CM.
 * TODO: Consider unification, now the status list is also in DB... 
 */
void CSnmpValue::parseStatusValue() {

	cout << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid();
	cout << " snmp:" << m_snmpOid.getProperty(KEY_PARAM_OID_LABEL) << " raw:" << m_raw_value << endl;

	map<int, string> map_status;

	map_status[1] = "other";
	map_status[2] = "ranging";
	map_status[3] = "rangingAborted";
	map_status[4] = "rangingComplete";
	map_status[5] = "ipComplete";
	map_status[6] = "registrationComplete";
	map_status[7] = "accessDenied";

	m_value = map_status[atoi(m_raw_value.c_str())];
}

/**
 * Parse the snmp value and set proper final value.
 */
void CSnmpValue::parseSnmpValue() {

	stringstream ss;

	m_value = m_raw_value;

	replace(m_value.begin(), m_value.end(), '\'', ' ');
	replace(m_value.begin(), m_value.end(), '\\', ' ');

	m_iValue = atol(m_value.c_str());
	m_fValue = atof(m_value.c_str());

	// TODO: make it better... Decide which type of Snmp Value is
	// If dimension is not defined - use string
	if (m_snmpOid.getProperty(KEY_PARAM_OID_DIM).size() == 0) {

		// Check and parse if it is a Status
		if (m_snmpOid.getProperty(KEY_PARAM_OID_LABEL).compare(KEY_COL_CM_STATUS) == 0) {
			parseStatusValue();
		}

	} else {
		// DIMensional params

		double mult = atof(m_snmpOid.getProperty(KEY_PARAM_OID_MULT).c_str());

		if (m_raw_value.find_first_of('.') == string::npos && mult == 1) {
			// Current value without change is used.
			ss << m_iValue;

		} else {

			// Check if 64 bit counters are used. Should use better precision.
			if (m_snmpOid.getProperty(KEY_PARAM_OID_VAL_TYPE).find("Counter64") != string::npos) {

				double dmult = 1 / mult;

				m_iValue = m_iValue / ((long) dmult);

				ss << m_iValue;

			} else {
				// Multiply value
				m_fValue = m_fValue * mult;
				ss << m_fValue;
			}


		}
		
		m_value = ss.str();
	}

}

const string& CSnmpValue::getRawValue() const {
	return m_raw_value;
}
const string& CSnmpValue::getValue() const {
	return m_value;
}
const long CSnmpValue::getValueInt() const {
	return m_iValue;
}
const float CSnmpValue::getValueFloat() const{
	return m_fValue;
}

/**
 * Check is the value = 0
 */
bool CSnmpValue::isZero() const {
	return getValue().compare("0") == 0;
}

/**
 * Append Dumped Snmp Value to a StringStream.
 * It can be used to display the snmp
 */
void CSnmpValue::dumpStrInfo(stringstream& result) const {

	result << m_snmpOid.getProperty(KEY_PARAM_OID_LABEL) << ": ";
	result << m_value << " " << m_snmpOid.getProperty(KEY_PARAM_OID_DIM);

}
/**
 * Append Dumped Snmp Value info to a string.
 * It can be used to display the snmp
 */
void CSnmpValue::dumpStrInfo(string& result) const {

	stringstream ss;

	dumpStrInfo(ss);

	result += ss.str();
}

std::string CSnmpValue::toXml() const {
	stringstream ss;
	toXml(ss);
	return ss.str();
}

/**
 * SnmpValue as an XML stream.
 *
 * Consider make it automated key->value xml-ing...
 */
void CSnmpValue::toXml(std::stringstream& result) const {
	result << "<snmpvalue>";
	result << "<oidid>" << m_snmpOid.getProperty(KEY_PARAM_OID_ID) << "</oidid>";
	result << "<oidlabel>" << m_snmpOid.getProperty(KEY_PARAM_OID_LABEL) << "</oidlabel>";
	result << "<value>" << m_value << "</value>";
	result << "<oiddim>" << m_snmpOid.getProperty(KEY_PARAM_OID_DIM) << "</oiddim>";
	result << "</snmpvalue>";
}

void CSnmpValue::toXml(std::string& result) const {
	stringstream ss;
	toXml(ss);
	result += ss.str();
}

/**
 * Dump SnmpValue as a Function Point in XML.
 * Should contain value vs time
 */
void CSnmpValue::toFnPointXml(std::string& result) const {
	stringstream ss;

	ss << "<point>";
	ss << "<labelx>time</labelx>";
	ss << "<labely>" << m_snmpOid.getProperty(KEY_PARAM_OID_LABEL) << "</labely>";
	ss << "<x>" << this->getTimeStampUx() << "</x>";
	ss << "<y>" << m_value << "</y>";
	ss << "<dimy>" << m_snmpOid.getProperty(KEY_PARAM_OID_DIM) << "</dimy>";
	ss << "</point>";

	result += ss.str();
}

void CSnmpValue::dumpInfo() {
	cout << endl << "--------------------------" << endl;
	cout << __FILE__ << ":" << __FUNCTION__ << endl;

	cout << "raw value:" << m_raw_value << endl;
	cout << "value:s:" << m_value << endl;
	cout << "value:i:" << m_iValue << endl;
	cout << "value:f:" << m_fValue << endl;

	m_snmpOid.dumpInfo();

	cout << endl << "--------------------------" << endl;
}

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */
