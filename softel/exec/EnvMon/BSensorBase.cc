/*
 * BSensorBase.cc
 *
 *  Created on: 12.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BSensorBase.h"

using namespace std;

namespace SF {
namespace OSS {
namespace EXEC {

BSensorBase::BSensorBase() {
	this->setValue("0");
}

BSensorBase::BSensorBase(const string& id, const string& value) {
	this->setId(id);
	this->setValue(value);
}

BSensorBase::~BSensorBase() {
	// TODO Auto-generated destructor stub
}

void BSensorBase::setValue(const string& value) {
	this->m_value = value;
}

void BSensorBase::setId(const string& id) {
	this->m_id = id;
}

const string BSensorBase::getValue() const {
	return this->m_value;
}

const string BSensorBase::getId() const {
	return this->m_id;
}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */



