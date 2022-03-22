/*
 * ServerHttpWeb.cc
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include <iostream>
#include "ServerHttpWeb.h"
#include "RequestHandlerHttp.h"
#include "BPluginBase.h"
#include "PluginInfo.h"
#include <softel/core/Ptr.h>
#include <softel/db/CParamSet.h>
#include "PluginContainerWebSrvSingleton.h"

using namespace std;
using namespace SF::CORE::NET;

namespace SF {
namespace WEB {

ServerHttpWeb::ServerHttpWeb(const std::string& address, const std::string& port,
       const std::string& doc_root, std::size_t thread_pool_size)
   : ServerHttpBase(address, port, thread_pool_size),
     m_doc_root(doc_root) {

   cout << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << endl;

   m_request_handler.reset(new RequestHandlerHttp(doc_root));

   // Subscribe web srv general info Plugin
   SF::DB::CParamSet params;
   params.setParam("info", "This is ServerHttpWeb running on " + port + " port");
   params.setParam("port", port);
   params.setParam("doc_root", doc_root);

   SF::CORE::Ptr<BPluginBase> plugin(new PluginInfo(params));
   PluginContainerWebSrvSingleton::getInstance()->push_back(plugin->getKey(), plugin);
}

ServerHttpWeb::~ServerHttpWeb() {
   // TODO Auto-generated destructor stub
}

} /* namespace WEB */
} /* namespace SF */
