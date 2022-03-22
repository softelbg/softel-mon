/*
 * CParamSet.cc
 *
 *  Created on: 01.08.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CParamSet.h"

#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

namespace SF {
namespace DB {

CParamSet::CParamSet() {

}

CParamSet::~CParamSet() {

}


void CParamSet::setParam(const string& key, const string& val) {
	m_param_set[key] = val;
}

void CParamSet::setParam(const string& key, int val) {

	stringstream ss;

	ss << val;

	setParam(key, ss.str());
}

void CParamSet::setParam(const string& key, long val) {

	stringstream ss;

	ss << val;

	setParam(key, ss.str());
}

string CParamSet::getParam(const string& key) const {

	string val;

	getParam(key, val);

	return val;
}

void CParamSet::clear() {
	m_param_set.clear();
}

size_t CParamSet::size() const {
	return m_param_set.size();
}

bool CParamSet::getParam(const string& key, string& val) const {

	map<string, string>::const_iterator it = m_param_set.find(key);

	if (it != m_param_set.end()) {
		val = it->second;
		return true;
	} else {
		val = "";
		return false;
	}
}

bool CParamSet::hasParam(const string& key) const {

	string val;

	return this->getParam(key, val);
}

void CParamSet::getKeyValueList(std::vector<std::string>& keys,
		std::vector<std::string>& values) const {

	keys.clear();
	values.clear();

	map<string, string>::const_iterator it;

	for(it = this->m_param_set.begin(); it != this->m_param_set.end(); it++) {
		keys.push_back(it->first);
		values.push_back(it->second);
	}

}

std::string CParamSet::toString() const {
	stringstream ss;

	map<string, string>::const_iterator it;

	for(it = this->m_param_set.begin(); it != this->m_param_set.end(); it++) {
		ss << it->first << " = " << it->second << endl;
	}

	return ss.str();
}
void CParamSet::dumpInfo() const {
	cout << toString();
}


} /* namespace DB */
} /* namespace SF */

