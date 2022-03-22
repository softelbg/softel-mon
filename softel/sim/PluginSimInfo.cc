/*
 * PluginSimInfo.cc
 *
 *  Created on: Apr 18, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "PluginSimInfo.h"
#include <softel/sim/SimDbPool.h>
#include <map>
#include <iostream>

using namespace std;
using namespace SF::CORE;
using namespace SF::CORE::POOL;

namespace SF {
namespace SIM {

PluginSimInfo::PluginSimInfo()  :
	BPluginBase("PluginSimInfo") {

}

PluginSimInfo::~PluginSimInfo() {

}

/**
 * Subscribe any object
 */
void PluginSimInfo::subscribe(Ptr<IObjectInterface> s) {
	m_subscribers.push_back(s);
}

void PluginSimInfo::run(const SF::CORE::NET::RequestHttpBase& req) {

	cout << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << endl;

	setResult(__FILE__"\n");

	for(size_t i = 0; i < m_subscribers.size(); i++) {
		appendResult(m_subscribers[i]->toXml() + "\n");
	}

}

} /* namespace SIM */
} /* namespace SF */
