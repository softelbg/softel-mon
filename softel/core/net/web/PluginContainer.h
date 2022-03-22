/*
 * PluginContainer.h
 *
 *  Created on: Apr 8, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef PluginContainer_H_
#define PluginContainer_H_

#include "BPluginBase.h"
#include <softel/core/Ptr.h>
#include <softel/core/pool/RContainer.h>
#include <softel/core/net/server/RequestHttpBase.h>

#include <string>
#include <vector>
#include <map>

namespace SF {
namespace WEB {

class PluginContainer: public 	SF::CORE::POOL::RContainer<SF::CORE::Ptr<BPluginBase>> {
public:
   PluginContainer();
   virtual ~PluginContainer();

   virtual bool run(const SF::CORE::NET::RequestHttpBase& req, std::string& result);

};

} /* namespace WEB */
} /* namespace SF */
#endif /* PluginContainer_H_ */
