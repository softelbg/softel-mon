/*
 * BSnmpParamBase.h
 *
 *  Created on: 19.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BSNMPPARAMBASE_H_
#define BSNMPPARAMBASE_H_

#include <softel/db/CParamSet.h>
#include <softel/db/CDataRow.h>

namespace SF {
namespace OSS {
namespace SNMP {

class BSnmpParamBase {
public:
	BSnmpParamBase();
	virtual ~BSnmpParamBase();

	virtual void init()=0;
	virtual void init(SF::DB::CDataRow& rowCmts);

	void clear();
	int size() const;

	void setParam(const std::string& key, const std::string& val);
	std::string getParam(const std::string& key) const;

protected:

	SF::DB::CDataRow m_row;

};

} /* namespace SNMP */
} /* namespace OSS */
} /* namespace SF */

#endif /* BSNMPPARAMBASE_H_ */
