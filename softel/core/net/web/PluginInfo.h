/*
 * PluginInfo.h
 *
 *  Created on: Apr 8, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef PluginInfo_H_
#define PluginInfo_H_

#include "BPluginBase.h"
#include <softel/core/net/server/RequestHttpBase.h>
#include <softel/db/CParamSet.h>

#include <string>

namespace SF {
namespace WEB {

class PluginInfo: public BPluginBase {
public:
   PluginInfo(SF::DB::CParamSet info);
   virtual ~PluginInfo();

   virtual void run(const SF::CORE::NET::RequestHttpBase& req);

private:
   SF::DB::CParamSet m_info;
};

} /* namespace WEB */
} /* namespace SF */
#endif /* PluginInfo_H_ */
