/*
 * RequestHandlerHttp.h
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#ifndef REQUESTHANDLERHTTP_H_
#define REQUESTHANDLERHTTP_H_

#include <boost/shared_ptr.hpp>
#include <softel/core/net/server/RequestHandlerBase.h>
#include <softel/core/net/server/RequestBase.h>
#include <softel/core/net/server/ReplyBase.h>

namespace SF {
namespace WEB {

class RequestHandlerHttp : public SF::CORE::NET::RequestHandlerBase {
public:
   explicit RequestHandlerHttp(const std::string& doc_root);
   virtual ~RequestHandlerHttp();

   virtual void handleRequest(
      const boost::shared_ptr<SF::CORE::NET::RequestBase>& req,
      boost::shared_ptr<SF::CORE::NET::ReplyBase>& rep);

private:
   std::string m_doc_root;
};

} /* namespace WEB */
} /* namespace SF */
#endif /* REQUESTHANDLERHTTP_H_ */
