/*
 * CThresholdBase.cc
 *
 *  Created on: 02.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CThresholdBase.h"

using namespace std;
using namespace SF::DB;

namespace SF {
namespace OSS {
namespace ALERT {

CThresholdBase::CThresholdBase() {
	// TODO Auto-generated constructor stub

}

CThresholdBase::CThresholdBase(const CDataRow& dr) {
	this->init(dr);
}

CThresholdBase::~CThresholdBase() {
	// TODO Auto-generated destructor stub
}

void CThresholdBase::init() {
}

void CThresholdBase::init(const CDataRow& dr) {
	this->m_data_row = dr;
}

string CThresholdBase::getParam(const string& key) {
	//return this->m_param_set.getParam(key);
	return this->m_data_row.getColumn(key);
}

float CThresholdBase::getValue() {
	return atof(this->getParam(PARAM_THRES_VALUE).c_str());
}

int CThresholdBase::getSeverity() {
	return atoi(this->getParam(PARAM_THRES_SEVERITY).c_str());
}

string CThresholdBase::getColor() {
	return this->getParam(PARAM_THRES_COLOR);
}

string CThresholdBase::getCondition() {
	return this->getParam(PARAM_THRES_CONDITION);
}

bool CThresholdBase::compare(const string& value) {
	return this->compare(atof(value.c_str()));
}

/**
 * Compare current value acconrding to threshold condition to threshold value.
 * Return true if the condition is met
 */
bool CThresholdBase::compare(float value) {

	bool result;

	float fTValue = this->getValue();

	string condition = this->getCondition();

	if (condition.compare(CONDITION_LESS) == 0) {
		result = (value < fTValue);
	} else if (condition.compare(CONDITION_MORE) == 0) {
		result = (value > fTValue);
	}

	return result;
}

void CThresholdBase::dumpInfo() const {
	this->m_data_row.dumpInfo();
}

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */



