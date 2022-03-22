/*
 * ServerHttpWeb.h
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef ServerHttpWeb_H_
#define ServerHttpWeb_H_

#include <softel/core/net/server/ServerHttpBase.h>

namespace SF {
namespace WEB {

class ServerHttpWeb: public SF::CORE::NET::ServerHttpBase {
public:
   explicit ServerHttpWeb(const std::string& address, const std::string& port,
               const std::string& doc_root, std::size_t thread_pool_size);
   virtual ~ServerHttpWeb();

private:
   std::string m_doc_root;
};

} /* namespace WEB */
} /* namespace SF */
#endif /* ServerHttpWeb_H_ */
