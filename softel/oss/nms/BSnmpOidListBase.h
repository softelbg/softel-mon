/*
 * BSnmpOidListBase.h
 *
 *  Created on: 04.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BSNMPOIDLISTBASE_H_
#define BSNMPOIDLISTBASE_H_

#include <vector>

#include <softel/core/intf/CListInterface.h>
#include <softel/oss/cmts/BSnmpOidBase.h>
#include <softel/oss/cmts/CSnmpOid.h>
#include <softel/db/CDataSet.h>
#include <softel/db/CDataRow.h>
#include <softel/db/CPosIndex.h>
#include "CSqlNmsTools.h"

namespace SF {
namespace OSS {
namespace NMS {

class BSnmpOidListBase: public SF::CORE::INTF::CListInterface {
public:
	BSnmpOidListBase();
	virtual ~BSnmpOidListBase();

	virtual void init(const SF::DB::CDataSet& ds);
	virtual void init();

	virtual void clear();
	virtual size_t size() const;

	void add(SF::OSS::CMTS::BSnmpOidBase* pSnmpOid);

	SF::OSS::CMTS::BSnmpOidBase* get(size_t pos);
	SF::OSS::CMTS::BSnmpOidBase* get(const std::string& key);

	virtual void getSnmpOidVector(std::vector<SF::OSS::CMTS::CSnmpOid>& oidList);

	virtual void dumpInfo() const;

protected:

	std::vector<SF::OSS::CMTS::BSnmpOidBase*> m_oid_list;
	SF::DB::CPosIndex m_idx;

private:

	void addIdx(SF::OSS::CMTS::BSnmpOidBase* pSnmpOid);

};

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */

#endif /* BSNMPOIDLISTBASE_H_ */
