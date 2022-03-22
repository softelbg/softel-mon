/*
 * CPingCmtsCm.cpp
 *
 *  Created on: 19.09.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CPingCmtsCm.h"

using namespace std;

namespace SF {
namespace CORE {
namespace NET {

#define MSG_LOSS		"Загуби към устройството - "
#define MSG_DELAY		"Забавяне на връзката към устройството - "

CPingCmtsCm::CPingCmtsCm(const std::string& ip)
	: CPing(ip) {

}

CPingCmtsCm::~CPingCmtsCm() {
	// TODO Auto-generated destructor stub
}

void CPingCmtsCm::setResponse() {

	this->m_response_final = getResult();

	// TODO: Rework here...
	if (this->m_loss >= 10) {
		std::stringstream ss;

		ss << "<font color=red>" << endl;
		//ss << "Ping Losses to " << this->getIp() << " ";
		ss << MSG_LOSS;
		ss << this->m_loss << "%" << endl;
		ss << "</font>";

		this->m_response_final += ss.str();
	}

	if (this->m_roundtrip_max >= 50) {
		std::stringstream ss;

		ss << "<font color=red>" << endl;
		//ss << "Ping Roundtrip too long: ";
		ss << MSG_DELAY;
		ss << this->m_roundtrip_max;
		ss << " ms" << endl;
		ss << "</font>";

		this->m_response_final += ss.str();
	}

}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
