/*
 * BSnmpOidBase.h
 *
 *  Created on: 04.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BSNMPOIDBASE_H_
#define BSNMPOIDBASE_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>

#include <softel/db/CDataRow.h>
#include "SnmpOids.h"

namespace SF {
namespace OSS {
namespace CMTS {

class BSnmpOidBase {
public:
	BSnmpOidBase();
	BSnmpOidBase(const SF::DB::CDataRow& row);
	virtual ~BSnmpOidBase();

	void init(const SF::DB::CDataRow& row);
	virtual void initFromDB()=0;

	virtual void addProperty(const std::string& key, const std::string& value);
	virtual std::string getProperty(const std::string& key) const;
	virtual bool getProperty(const std::string& key, std::string& result) const;

	void setSnmpOid(const char*);
	void setSnmpOid(const std::string&);

	void setOidId(const int);
	void setOidId(const char*);
	void setOidId(const std::string&);

	std::string getSnmpOid() const;
	int getOidId() const;
	std::string getOidIdStr() const;

	virtual void dumpInfo() const;

protected:

	SF::DB::CDataRow	m_oid_params;

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif /* BSNMPOIDBASE_H_ */
