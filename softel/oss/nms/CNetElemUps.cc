/*
 * CNetElemUps.cc
 *
 *  Created on: 25.02.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CNetElemUps.h"

using namespace std;
using namespace SF::DB;

namespace SF {
namespace OSS {
namespace NMS {

CNetElemUps::CNetElemUps(const string& ip) {

	this->addIf(new CNetworkElementIF(ip));

}

CNetElemUps::CNetElemUps(const CDataRow& row) :
	BNetworkElementBase(row) {

}

CNetElemUps::~CNetElemUps() {
	// TODO Auto-generated destructor stub

	this->getIfList().clear();

}

CNetElemUps::CNetElemUps(const string& ip, const string& mac) {

	this->addIf(new CNetworkElementIF(ip, mac));

}

void CNetElemUps::init() {

}

} /* namespace NMS */
} /* namespace OSS */
} /* namespace SF */


