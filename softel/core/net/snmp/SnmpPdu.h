/*
 * SnmpPdu.h
 *
 *  Created on: 20.09.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef SNMPPDU_H_
#define SNMPPDU_H_

#include <softel/core/CObject.h>
#include "SnmpPackageBuilder.h"
#include <softel/core/util/DateTime.h>
#include "SnmpPduVar.h"
#include <string>

namespace SF {
namespace CORE {
namespace NET {

/*
 *
 */
class SnmpPdu: public SF::CORE::CObject {
public:
	SnmpPdu();
	virtual ~SnmpPdu();

	bool AddVar(const std::string& oid, const std::string& value, int maxlen);
	bool AddVar(const std::string& oid, const int& value);
	bool AddVar(const std::string& oid, const SF::CORE::UTIL::DateTime& value);

	bool BuildPackage(SnmpPackageBuilder& builder) const {
		int length = builder.GetSize();
		for (int i = m_variables.size() - 1; i >= 0; i--) {
			SnmpPduVar *var = m_variables[i];
			if (!var->BuildPackage(builder)) {
				return false;
			}
		}
		length = builder.GetSize() - length;
		return builder.PushSequence(ASN_TYPE_SEQUENCE, length);
	}

private:
	std::vector<SnmpPduVar*> m_variables;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* SNMPPDU_H_ */
