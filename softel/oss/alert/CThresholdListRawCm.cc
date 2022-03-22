/*
 * CThresholdListRawCm.cc
 *
 *  Created on: 03.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CThresholdListRawCm.h"

using namespace std;
using namespace SF::DB;

namespace SF {
namespace OSS {
namespace ALERT {

CThresholdListRawCm::CThresholdListRawCm() {
	// TODO Auto-generated constructor stub

}

CThresholdListRawCm::~CThresholdListRawCm() {
	// TODO Auto-generated destructor stub
}

void CThresholdListRawCm::init() {

	CSqlThresholdTools sql;

	sql.sqlSelectThresholdListCm(this->m_data_set);

}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */


