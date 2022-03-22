/*
 * PluginSimTest.cc
 *
 *  Created on: Apr 18, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "PluginSimTest.h"
#include <softel/sim/SimDbPool.h>
#include <map>
#include <iostream>

using namespace std;
using namespace SF::CORE::POOL;

namespace SF {
namespace SIM {

PluginSimTest::PluginSimTest()  :
	BPluginBase("PluginSimTest") {
	// TODO Auto-generated constructor stub

}

PluginSimTest::~PluginSimTest() {
	// TODO Auto-generated destructor stub
}

void PluginSimTest::run(const SF::CORE::NET::RequestHttpBase& req) {

	cout << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << endl;

	setResult(__FILE__"\n");

	map<string, string>::const_iterator it;

	appendResult("Request Args\n");
	for(it = req.m_arguments.begin(); it != req.m_arguments.end(); it++) {
		appendResult(it->first + " = " + it->second + "\n");
	}

	DbResult r;

	//SimDbPool::getInstance()->doSelect("SELECT * FROM SIM_TEST", r);
	//appendResult("\n\n" + r.toString());

	it = req.m_arguments.find("table");
	if (it != req.m_arguments.end()) {
		SimDbPool::getInstance()->doSelect("SELECT count(*) FROM " + it->second, r);
		appendResult("\n\n" + r.toString());

		SimDbPool::getInstance()->doSelect("SELECT * FROM " + it->second, r);
		appendResult("\n\n" + r.toString());
	}

}

} /* namespace SIM */
} /* namespace SF */
