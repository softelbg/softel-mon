/*
 * PluginContainer.cc
 *
 *  Created on: Apr 8, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "PluginContainer.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;
using namespace SF::CORE::NET;

namespace SF {
namespace WEB {

PluginContainer::PluginContainer() {

}

PluginContainer::~PluginContainer() {
   // TODO Auto-generated destructor stub
}

bool PluginContainer::run(const RequestHttpBase& req, std::string& result) {

	// TODO: make request plugin routing here....

	string key;

	// TODO: Rework...
	size_t pos = req.uri_file.find("/sfplugin/");
	if (pos != string::npos) {
		pos += 10;
		key = req.uri_file.substr(pos);
	}

	cout << __FILE__ << "::" << __FUNCTION__ << " key: " << key << endl;

	SF::CORE::Ptr<BPluginBase> plugin;
	if (get(key, plugin)) {
		plugin->run(req);
		plugin->getResult(result);
		return true;
	} else {
		return false;
	}
}

} /* namespace WEB */
} /* namespace SF */
