/*
 * PluginInfo.cc
 *
 *  Created on: Apr 8, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "PluginInfo.h"

#include <string>

using namespace std;
using namespace SF::CORE;
using namespace SF::CORE::NET;

namespace SF {
namespace WEB {

PluginInfo::PluginInfo(SF::DB::CParamSet info) :
	BPluginBase("PluginInfo"),
	m_info(info) {

}

PluginInfo::~PluginInfo() {
   // TODO Auto-generated destructor stub
}

void PluginInfo::run(const RequestHttpBase& req) {

	setResult("This is the test run of the PluginInfo: " + getName() + "\n<br>");

	appendResult("HttpRequest: ");
	appendResult(req.toString());

	appendResult("Remote IP: ");
	appendResult(req.getRemoteIp());

	// Some info...
	string reqInfo;
	if (m_info.getParam("info", reqInfo)) {
		appendResult("Info: " + reqInfo);
	}
	appendResult("m_info: ");
	appendResult(m_info.toString());

}

} /* namespace WEB */
} /* namespace SF */
