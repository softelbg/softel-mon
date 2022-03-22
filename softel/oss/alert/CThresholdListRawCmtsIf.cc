/*
 * CThresholdListRawCmtsIf.cc
 *
 *  Created on: 03.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CThresholdListRawCmtsIf.h"

using namespace std;
using namespace SF::DB;

namespace SF {
namespace OSS {
namespace ALERT {

CThresholdListRawCmtsIf::CThresholdListRawCmtsIf() {
	// TODO Auto-generated constructor stub

}

CThresholdListRawCmtsIf::~CThresholdListRawCmtsIf() {
	// TODO Auto-generated destructor stub
}

void CThresholdListRawCmtsIf::init() {

	CSqlThresholdTools sql;

	sql.sqlSelectThresholdListIf(this->m_data_set);

}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */


