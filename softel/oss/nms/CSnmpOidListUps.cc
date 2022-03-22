/*
 * CSnmpOidListUps.cc
 *
 *  Created on: 04.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSnmpOidListUps.h"

#include <softel/db/CDataSet.h>

using namespace std;
using namespace SF::DB;

namespace SF {
namespace OSS {
namespace NMS {

CSnmpOidListUps::CSnmpOidListUps() {

	this->init();

}

CSnmpOidListUps::~CSnmpOidListUps() {
	// TODO Auto-generated destructor stub
}

void CSnmpOidListUps::init() {

	CDataSet ds;

	CSqlNmsTools sqlTools;

	sqlTools.selectListSnmpOidNetElemUps(ds);

	BSnmpOidListBase::init(ds);

}

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */


