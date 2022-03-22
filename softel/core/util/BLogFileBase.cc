/*
 * BLogFileBase.cc
 *
 *  Created on: 03.04.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BLogFileBase.h"
#include "CStringUtil.h"
#include <softel/core/system/CSystemTools.h>

using namespace std;
using namespace SF::CORE::SYSTEM;

namespace SF {
namespace CORE {
namespace UTIL {

BLogFileBase::BLogFileBase(const std::string& logFileName) :
		BFileBase(logFileName) {

	this->setMode(ios_base::in | ios_base::out | ios_base::app);

	this->openFile();
}

BLogFileBase::~BLogFileBase() {
	// TODO Auto-generated destructor stub
}

void BLogFileBase::log(const std::string& message) {
	this->log(LOG_LEVEL_INFO, message);
}

/**
 * Logging Time Stamp: LOG LEVEL : Message
 */
void BLogFileBase::log(int level, const std::string& message) {

	this->write(this->doLogLine(level, message));

}

void BLogFileBase::log(const std::string& logFileName,
		const std::string& message) {
	BLogFileBase::log(logFileName, LOG_LEVEL_INFO, message);
}

void BLogFileBase::log(const std::string& logFileName, int level,
		const std::string& message) {

	BLogFileBase logFile(logFileName);
	logFile.log(level, message);

}

/**
 * Do LOG Line. Could be overriden in subclasses...
 */
string BLogFileBase::doLogLine(int level, const std::string& message) {

	// TODO: Make proper log level messages handler...
	std::map<int, string> listLogLevels;
	listLogLevels[LOG_LEVEL_DEBUG] = "DEBUG";
	listLogLevels[LOG_LEVEL_INFO] = "INFO";
	listLogLevels[LOG_LEVEL_WARN] = "WARN";
	listLogLevels[LOG_LEVEL_ERROR] = "ERROR";

	std::stringstream ss;

	std::string timeStamp = CSystemTools::getCTimeNow();
	CStringUtil::remove("\r\n", timeStamp);

	ss << timeStamp << " " << listLogLevels[level] << ": " << message << std::endl;

	return ss.str();
}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

