/*
 * BFileReaderBase.cc
 *
 *  Created on: 18.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BFileReaderBase.h"

using namespace std;

namespace SF {
namespace CORE {
namespace UTIL {

BFileReaderBase::BFileReaderBase() {

}

BFileReaderBase::BFileReaderBase(const std::string& fileName) :
		BFileBase(fileName) {

}

BFileReaderBase::~BFileReaderBase() {
	// TODO Auto-generated destructor stub
}

void BFileReaderBase::read() {

	std::string line;

	std::ifstream fileIfStream(this->getFileName().c_str());

	if (fileIfStream.is_open()) {

		while (fileIfStream.good()) {

			std::getline(fileIfStream, line);

			processLine(line);
		}

		fileIfStream.close();
	} else {
		std::cout << "Error open " << this->getFileName() << std::endl;
	}
}

void BFileReaderBase::write() {

}

void BFileReaderBase::init() {
	this->read();
}

void BFileReaderBase::processLine(const std::string& line) {
	std::cout << "Line:" << line << std::endl;
}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

