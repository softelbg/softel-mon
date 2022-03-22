/*
 * CMonDeviceHttpBlizoo.cc
 *
 *  Created on: 12.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include <vector>
#include "CMonDeviceHttpBlizoo.h"
#include <softel/core/util/CStringUtil.h>
#include <softel/core/http/CHttpRequest.h>
#include <softel/core/http/CHttpResponse.h>

using namespace std;
using namespace SF::OSS::EXEC;
using namespace SF::CORE::NET;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::HTTP;

namespace SF {
namespace OSS {
namespace EXEC {

CMonDeviceHttpBlizoo::CMonDeviceHttpBlizoo(const CHost& host) :
	CMonDeviceBase(host) {

	this->setUrl("/index.htm?sensors=on");
}

CMonDeviceHttpBlizoo::CMonDeviceHttpBlizoo(const CHost& host,
		const string& url) :
		CMonDeviceBase(host) {

	this->setUrl(url);
}

CMonDeviceHttpBlizoo::~CMonDeviceHttpBlizoo() {

	this->clear();

}

void CMonDeviceHttpBlizoo::sendRequest() {
	this->sendHttpRequest();
}

string CMonDeviceHttpBlizoo::getResult() {
	return this->m_result;
}

/**
 * Parse the Result Response.
 */
void CMonDeviceHttpBlizoo::parseResult() {

	cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	string rawResponse = this->getResponse();

	vector<string> listLines;

	CStringUtil::tokenize(rawResponse, "\n", listLines);

	for(size_t i = 0; i < listLines.size(); i++) {
		this->parseResultLine(listLines[i]);
	}

}

/**
 * Parse single Response Line.
 * Should extract sensor id and value. Store them in a Map.
 *
 */
void CMonDeviceHttpBlizoo::parseResultLine(const string& line) {

	cout << __FILE__ << ":" << __FUNCTION__ << " line:" << line << endl;

	string sensorId;
	string sensorValue;

	int posCurrent;

	if (CStringUtil::findFirstMiddleSubstring(line, "<", ">", 0, sensorId, &posCurrent)) {

		if (CStringUtil::findFirstMiddleSubstring(line, " ", " ", posCurrent, sensorValue, &posCurrent)) {

			this->m_sensor_array[sensorId] = new CSensorTekom(sensorId, sensorValue);

		} else {

		}

	} else {

	}

}

void CMonDeviceHttpBlizoo::run() {

	cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	this->sendRequest();

}

void CMonDeviceHttpBlizoo::clear() {

	cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	map<string, CSensorTekom*>::const_iterator it;

	for (it = this->m_sensor_array.begin(); it != this->m_sensor_array.end(); it++) {
		free(it->second);
	}

}

int CMonDeviceHttpBlizoo::size() const {
	return this->m_sensor_array.size();
}

void CMonDeviceHttpBlizoo::sendHttpRequest() {

	cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << endl;

	string httpUrl;

	httpUrl = this->m_url + " HTTP/1.1\r\n\r\n";

	CHttpRequest httpRequest(this->getHost(), httpUrl);

	if (httpRequest.doGet()) {

		CHttpResponse& httpResponse = httpRequest.getHttpResponse();

		this->setRawResponse(httpResponse.getBody());

		this->parseResult();

	} else {
		CHttpResponse& httpResponse = httpRequest.getHttpResponse();
		this->setRawResponse(httpResponse.getError());
	}

}


void CMonDeviceHttpBlizoo::setUrl(const string& url) {
	this->m_url = url;
}

void CMonDeviceHttpBlizoo::dumpInfo() {

	cout << __FILE__ << ": result[" << this->getResult() << "]" << endl;
	cout << __FILE__ << ": response[" << this->getResponse() << "]" << endl;

	map<string, CSensorTekom*>::const_iterator it;

	for(it = this->m_sensor_array.begin(); it != this->m_sensor_array.end(); it++) {
		//cout << "Sensor: " << it->first << " = " << it->second->getValue() << endl;
		it->second->dumpInfo();
	}

}

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */


