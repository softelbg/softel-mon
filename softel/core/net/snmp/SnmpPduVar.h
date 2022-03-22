/*
 * SnmpPduVar.h
 *
 *  Created on: 20.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef SNMPPDUVAR_H_
#define SNMPPDUVAR_H_

#include <softel/core/CObject.h>
#include "SnmpPackageBuilder.h"
#include "SnmpSession.h"

namespace SF {
namespace CORE {
namespace NET {

/*
 *
 */
class SnmpPduVar: public SF::CORE::CObject {
public:
	SnmpPduVar(const std::string& oid) :
			m_oid(oid) {
	}
	virtual ~SnmpPduVar() {
	}

	virtual bool BuildPackage(SnmpPackageBuilder& builder) {
		int length = builder.GetSize();
		if (!BuildData(builder) || !BuildName(builder)) {
			return false;
		}
		length = builder.GetSize() - length;
		return builder.PushSequence(ASN_TYPE_SEQUENCE, length);
	}

	virtual bool BuildName(SnmpPackageBuilder& builder) {
		return builder.PushOID(ASN_TYPE_OID, m_oid);
	}

	virtual bool BuildData(SnmpPackageBuilder& builder) = 0;

protected:
	std::string m_oid;
};

/**
 *---------------------------------------------------------------------
 *
 * class SnmpPduVarString --
 *
 *    String overload for a SNMP PDU varbind.
 *
 *---------------------------------------------------------------------
 */
class SnmpPduVarString: public SnmpPduVar {
public:
	SnmpPduVarString(const std::string& oid, const std::string& str) :
			SnmpPduVar(oid), m_str(str) {
	}

	virtual bool BuildData(SnmpPackageBuilder& builder) {
		return builder.PushString(ASN_TYPE_STRING, m_str);
	}

private:
	std::string m_str;
};

/**
 *---------------------------------------------------------------------
 *
 * class SnmpPduVarInteger --
 *
 *    Integer overload for a SNMP PDU varbind.
 *
 *---------------------------------------------------------------------
 */

class SnmpPduVarInteger: public SnmpPduVar {
public:
	SnmpPduVarInteger(const std::string& oid, const int& var) :
		SnmpPduVar(oid), m_var(var) {
	}

	virtual bool BuildData(SnmpPackageBuilder& builder) {
		return builder.PushLong(ASN_TYPE_INTEGER, m_var);
	}

private:
	int m_var;
};

/**
 *---------------------------------------------------------------------
 *
 * class SnmpPduVarDateTime --
 *
 *    Vmomi::DateTime overload for a SNMP PDU varbind.
 *
 *---------------------------------------------------------------------
 */

class SnmpPduVarDateTime: public SnmpPduVar {
public:
	SnmpPduVarDateTime(const std::string& oid, const SF::CORE::UTIL::DateTime& var) :
		SnmpPduVar(oid), m_var(var) {
	}

	virtual bool BuildData(SnmpPackageBuilder& builder) {
		return builder.PushDateTime(ASN_TYPE_STRING, m_var);
	}

private:
	SF::CORE::UTIL::DateTime m_var;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* SNMPPDUVAR_H_ */
