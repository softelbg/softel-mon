/*
 * BPluginBase.h
 *
 *  Created on: Apr 8, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BPluginBase_H_
#define BPluginBase_H_

#include <softel/core/CObject.h>
#include <softel/core/net/server/RequestHttpBase.h>
#include <softel/core/Ptr.h>

#include <string>

namespace SF {
namespace WEB {

class BPluginBase: public SF::CORE::CObject {
public:
   BPluginBase(const std::string& pluginName);
   virtual ~BPluginBase();

   virtual void run(const SF::CORE::NET::RequestHttpBase& req) = 0;

   virtual std::string getKey() const;

   std::string getName() const;

   void setResult(const std::string& result);
   void appendResult(const std::string& result);
   std::string getResult() const;
   void getResult(std::string& result) const;

private:
   std::string m_plugin_name;
   std::string m_result;
};

} /* namespace WEB */
} /* namespace SF */
#endif /* BPluginBase_H_ */
