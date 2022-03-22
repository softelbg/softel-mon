/*
 * CThresholdListRaw.cc
 *
 *  Created on: 02.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CThresholdListRaw.h"

using namespace std;
using namespace SF::DB;

namespace SF {
namespace OSS {
namespace ALERT {

CThresholdListRaw::CThresholdListRaw() {
	// TODO Auto-generated constructor stub
}

CThresholdListRaw::~CThresholdListRaw() {
	// TODO Auto-generated destructor stub
}

void CThresholdListRaw::init() {

	CSqlThresholdTools sql;

	sql.sqlSelectThresholdList("", this->m_data_set);

}

void CThresholdListRaw::clear() {
	this->m_data_set.clear();
}

int CThresholdListRaw::size() const {
	return this->m_data_set.size();
}

const CDataSet& CThresholdListRaw::getRawDataSet() const {
	return this->m_data_set;
}

void CThresholdListRaw::dumpInfo() const {

	this->m_data_set.dumpInfo();

}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */



