/*
 * CThresholdListBase.cc
 *
 *  Created on: 03.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CThresholdListBase.h"

using namespace std;
using namespace SF::DB;

namespace SF {
namespace OSS {
namespace ALERT {

CThresholdListBase::CThresholdListBase() {
	// TODO Auto-generated constructor stub

}

CThresholdListBase::~CThresholdListBase() {
	// TODO Auto-generated destructor stub
}

void CThresholdListBase::clear() {
	this->m_list_thresholds.clear();
}

int CThresholdListBase::size() const {
	return this->m_list_thresholds.size();
}

/**
 * Add new threshold from a data row.
 */
void CThresholdListBase::add(const CDataRow& dr) {

	CThreshold threshold(dr);

	this->m_list_thresholds.push_back(threshold);
}

void CThresholdListBase::dumpInfo() const {

	for (int i = 0; i < this->m_list_thresholds.size(); i++) {

		cout << "T[" << i << "] = ";

		this->m_list_thresholds[i].dumpInfo();

		cout << endl;
	}

}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */
