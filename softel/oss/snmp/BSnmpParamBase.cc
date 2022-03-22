/*
 * BSnmpParamBase.cc
 *
 *  Created on: 19.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BSnmpParamBase.h"

using namespace std;
using namespace SF::DB;

namespace SF {
namespace OSS {
namespace SNMP {

BSnmpParamBase::BSnmpParamBase() {
	// TODO Auto-generated constructor stub

}

BSnmpParamBase::~BSnmpParamBase() {
	// TODO Auto-generated destructor stub
}

void BSnmpParamBase::clear() {
	this->m_row.clear();
}

int BSnmpParamBase::size() const {
	return this->m_row.size();
}

void BSnmpParamBase::setParam(const string& key, const string& val) {
	this->m_row.addColumn(key, val);
}

void BSnmpParamBase::init(CDataRow& rowCmts) {
	rowCmts.copyRow(this->m_row);
}

string BSnmpParamBase::getParam(const string& key) const {
	return this->m_row.getColumn(key);
}

} /* namespace SNMP */
} /* namespace OSS */
} /* namespace SF */


