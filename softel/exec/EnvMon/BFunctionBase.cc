/*
 * BFunctionBase.cc
 *
 *  Created on: 15.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BFunctionBase.h"

using namespace std;

namespace SF {
namespace OSS {
namespace EXEC {

BFunctionBase::BFunctionBase() {
	// TODO Auto-generated constructor stub

}

BFunctionBase::BFunctionBase(const double a1, const double b1, const int size1,
		const double a2, const double b2, const int size2, const string& dimension2) {

	this->setLinear(a1, b1, size1, a2, b2, size2, dimension2);
}

BFunctionBase::~BFunctionBase() {
	// TODO Auto-generated destructor stub
}

void BFunctionBase::setLinear(	const double a1, const double b1, const int size1,
								const double a2, const double b2, const int size2, const string& dimension2) {

	this->m_linear_a1 = a1;
	this->m_linear_b1 = b1;
	this->m_linear_size1 = size1;
	this->m_linear_a2 = a2;
	this->m_linear_b2 = b2;
	this->m_linear_size2 = size2;
	this->m_linear_dimension2 = dimension2;

}

string BFunctionBase::getLinearValueStr() {

	stringstream ss;

	ss << this->m_last_result;

	return ss.str();
}

string BFunctionBase::getLinearDimension() {
	return this->m_linear_dimension2;
}

string BFunctionBase::getLinearValueFormatedStr() {

	stringstream ss;

	ss << this->m_last_result << " " << this->getLinearDimension();

	return ss.str();
}

/**
 * Convert from Set1 -> Set2 a member of Set1.
 */
double BFunctionBase::convert(double x) {

	double y = -1;

	//double ab1 = this->m_linear_b1 - this->m_linear_a1;
	//double d1 = ab1/this->m_linear_size1;

	double ab2 = this->m_linear_b2 - this->m_linear_a2;
	double d2 = ab2/this->m_linear_size2;

	// Validate x belogns to Set1
	if (x >= this->m_linear_a1 && x <= this->m_linear_b1) {

	} else {
		x = this->m_linear_a1;
	}

	y = this->m_linear_a2 + x * d2;

	this->m_last_result = y;

	return y;
}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */


