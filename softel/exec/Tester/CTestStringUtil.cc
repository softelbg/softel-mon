/*
 * CTestStringUtil.cc
 *
 *  Created on: 14.01.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CTestStringUtil.h"

#include <softel/core/util/CStringUtil.h>

using namespace std;
using namespace SF::CORE::UTIL;

namespace SF {
namespace OSS {
namespace EXEC {

CTestStringUtil::CTestStringUtil() {
	// TODO Auto-generated constructor stub

}

CTestStringUtil::~CTestStringUtil() {
	// TODO Auto-generated destructor stub
}

void CTestStringUtil::run() {

	this->m_dec_mac = "154317135462";
	this->m_dec_mac_mark = "154317135462";
	this->m_hex_mac.clear();
	CStringUtil::convert2Base(this->m_dec_mac, 16, this->m_hex_mac);
	this->dumpInfo();

	this->m_dec_mac = "154317135462";
	this->m_dec_mac_mark = "154317135462";
	this->m_hex_mac.clear();
	CStringUtil::convertDecimal2Mac(this->m_dec_mac, ":", this->m_hex_mac);
	this->dumpInfo();

	this->m_dec_mac = "105737007295311";
	this->m_dec_mac_mark = "105737007295311";
	this->m_hex_mac.clear();
	this->m_hex_mac_mark = "60:2A:D0:C2:6B:4F";
	CStringUtil::convertDecimal2Mac(this->m_dec_mac, ":", this->m_hex_mac);
	this->dumpInfo();

	this->m_dec_mac.clear();
	this->m_dec_mac_mark = "105737007295311";
	this->m_hex_mac = "60:2A:D0:C2:6B:4F";
	this->m_hex_mac_mark = "60:2A:D0:C2:6B:4F";
	CStringUtil::convertMac2Dec(this->m_hex_mac, this->m_dec_mac);
	this->dumpInfo();

	this->m_dec_mac.clear();
	this->m_hex_mac = "60:2A:D0:C2:6B:4F";
	this->m_hex_mac_mark = "60:2A:D0:C2:6B:4F";
	CStringUtil::convertMac2DottedDecimal(this->m_hex_mac, ":", this->m_dec_mac);
	this->dumpInfo();

}

void CTestStringUtil::dumpInfo() {

	cout << __FILE__ << ":" << __LINE__ << ":";

	cout << " decMac: " << this->m_dec_mac;
	cout << " hexMac: " << this->m_hex_mac;

	cout << " decMacRes:" << this->m_dec_mac.compare(this->m_dec_mac_mark);
	cout << " hexMacRes:" << this->m_hex_mac.compare(this->m_hex_mac_mark);

	cout << endl;
}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */



