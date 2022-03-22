/*
 * BSnmpCmtsBase.cc
 *
 *  Created on: 14.01.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BSnmpCmtsBase.h"

#include <softel/oss/cmts/CCmtsToolsSql.h>
#include <softel/db/CMySqlDB.h>
#include <softel/db/CDataRow.h>
#include <softel/core/error/CErrorSql.h>

using namespace std;
using namespace SF::DB;
using namespace SF::OSS::CMTS;
using namespace SF::CORE::ERROR;

namespace SF {
namespace OSS {
namespace SNMP {

BSnmpCmtsBase::BSnmpCmtsBase() {
	// TODO Auto-generated constructor stub

}

BSnmpCmtsBase::BSnmpCmtsBase(CDataRow& rowCmts) {
	this->init(rowCmts);
}

BSnmpCmtsBase::~BSnmpCmtsBase() {
	// TODO Auto-generated destructor stub
}

void BSnmpCmtsBase::init() {
	this->initCmtsInfo();
}

void BSnmpCmtsBase::init(CDataRow& rowCmts) {
	BSnmpParamBase::init(rowCmts);
}

bool BSnmpCmtsBase::initCmtsInfo() {

	CCmtsToolsSql sqlTools;

	string cmtsId = this->getParam(KEY_COL_CMTS_ID);

	try {

		CDataRow rowCmts;

		sqlTools.sqlGetCmtsInfo(cmtsId, rowCmts);

		rowCmts.appendRow(this->m_row);

	} catch (CErrorSql& e) {
		return false;
	}

	return true;
}

} /* namespace SNMP */
} /* namespace OSS */
} /* namespace SF */

