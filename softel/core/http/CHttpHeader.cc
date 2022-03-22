/*
 * CHttpHeader.cc
 *
 *  Created on: 20.05.2012
 *      Author: STAN
 *
 *		www.softel.bg
 *
 *		Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CHttpHeader.h"

using namespace std;

namespace SF {
namespace CORE {
namespace HTTP {

CHttpHeader::CHttpHeader() {

}

CHttpHeader::CHttpHeader(const string& header) {
	setHeader(header);
}

void CHttpHeader::setHeader(const string& header) {

	m_header = header;

	parseHeader();

}

string& CHttpHeader::getHeader() {
	return m_header;
}

/**
 * Parse the header.
 * Set the header properties.
 */
void CHttpHeader::parseHeader() {

}

} /* namespace HTTP */
} /* namespace CORE */
} /* namespace SF */
