/*
 * BPluginBase.cc
 *
 *  Created on: Apr 8, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BPluginBase.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;
using namespace SF::CORE;
using namespace SF::CORE::NET;

namespace SF {
namespace WEB {

BPluginBase::BPluginBase(const std::string& pluginName) :
		m_plugin_name(pluginName) {
	cout << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << " m_plugin_name: " << m_plugin_name << endl;
}

BPluginBase::~BPluginBase() {
	// TODO Auto-generated destructor stub
}

string BPluginBase::getKey() const {
	cout << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << " m_plugin_name: " << m_plugin_name << endl;
	return m_plugin_name;
}

string BPluginBase::getName() const {
	return m_plugin_name;
}

void BPluginBase::setResult(const string& result) {
	m_result = result;
}

void BPluginBase::appendResult(const string& result) {
	m_result.append(result);
}

string BPluginBase::getResult() const {
	return m_result;
}

void BPluginBase::getResult(string& result) const {
	result = m_result;
}

} /* namespace WEB */
} /* namespace SF */
