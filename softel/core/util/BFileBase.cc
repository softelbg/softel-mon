/*
 * BFileBase.cc
 *
 *  Created on: 03.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BFileBase.h"

using namespace std;

namespace SF {
namespace CORE {
namespace UTIL {

BFileBase::BFileBase() {
	setMode(ios_base::in | ios_base::out | ios_base::app);
}

BFileBase::BFileBase(const std::string& fileName) {
	this->setFileName(fileName);
}

BFileBase::~BFileBase() {
	// TODO Auto-generated destructor stub
}

const std::string& BFileBase::getFileName() const {
	return this->m_file_name;
}

void BFileBase::setFileName(const std::string& fileName) {
	this->m_file_name = fileName;
}

void BFileBase::read() {

}

void BFileBase::read(std::string result) {

	this->openFile();

	if (this->is_open()) {

		std::getline(*this, result);

		this->close();
	} else {
		std::cout << "Error open " << this->getFileName() << std::endl;
	}

}

void BFileBase::write(const std::string& input) {

	this->openFile();

	if (this->is_open()) {

		*this << input;

		//this->close();
	} else {
		std::cout << "Error open " << this->getFileName() << std::endl;
	}
}

void BFileBase::setMode(const ios_base::openmode& mode) {
	this->m_mode = mode;
}

void BFileBase::openFile() {
	if (!this->is_open()) {
		this->open(this->getFileName().c_str(), this->m_mode);
	}
}

void BFileBase::closeFile() {
	if (this->is_open()) {
		this->close();
	}
}

void BFileBase::write() {

}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */


