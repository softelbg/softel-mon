/*
 * CSensorTekom.cc
 *
 *  Created on: 12.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CSensorTekom.h"

using namespace std;

namespace SF {
namespace OSS {
namespace EXEC {

CSensorTekom::CSensorTekom(const string& id, const string& value) :
	BSensorBase(id, value) {

	this->initConvertFunction();
	this->convert();
}

CSensorTekom::~CSensorTekom() {
	// TODO Auto-generated destructor stub
}

/**
 * Init List with convert functions.
 * Now the list is made here...
 * TODO: Should be from DB.
 *
 */
void CSensorTekom::initConvertFunction() {

	this->m_list_convert_function.clear();

	this->m_list_convert_function["1"] = new BFunctionBase(0, 3.2, 1024, 0, 10, 1024, "A");
	this->m_list_convert_function["2"] = new BFunctionBase(0, 3.2, 1024, 0, 250, 1024, "V");
	this->m_list_convert_function["3"] = new BFunctionBase(0, 3.2, 1024, 0, 30, 1024, "V");
	this->m_list_convert_function["4"] = new BFunctionBase(0, 3.2, 1024, -20, 80, 1024, "deg C");

}

string CSensorTekom::getFinalValue() {
	return this->m_final_value;
}

string CSensorTekom::getFinalValueFormated() {
	return this->m_final_value_formated;
}

string CSensorTekom::getDimension() {
	return this->m_dimension;
}

/**
 * Convert the raw value to a final value.
 * Should apply the convert functions from ADC read values to real physical params.
 */
void CSensorTekom::convert() {

	map<string, BFunctionBase*>::const_iterator it;

	it = this->m_list_convert_function.find(this->getId());

	if (it != this->m_list_convert_function.end()) {

		double x = atof(this->getValue().c_str());

		double y = it->second->convert(x);

		this->m_final_value = it->second->getLinearValueStr();
		this->m_final_value_formated = it->second->getLinearValueFormatedStr();
		this->m_dimension = it->second->getLinearDimension();

		cout << __FILE__ << ": X:" << x << " Y:" << y << " result:" << it->second->getLinearValueFormatedStr() << endl;

	} else {
		cout << __FILE__ << ": Error: Not Found: " << this->getId() << endl;
	}
}

void CSensorTekom::dumpInfo() {
	cout << __FILE__ << "[" << this->getId() << "]" << endl;
	cout << "val: " << this->getValue() << endl;
	cout << "Formated Final: " << this->getFinalValueFormated() << endl;
	cout << "Final: " << this->getFinalValue() << endl;
	cout << "Dimension: " << this->getDimension() << endl;
}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */
