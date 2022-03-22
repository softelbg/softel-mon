/*
 * RequestHandlerHttp.cc
 *
 *  Created on: Dec 4, 2013
 *      Author: sgeorgiev
 */

#include "RequestHandlerHttp.h"
#include <softel/core/net/server/RequestHttpBase.h>
#include <softel/core/net/server/RequestBase.h>
#include <softel/core/net/server/ProtoUtils.h>
#include <softel/core/net/server/ReplyHttpBase.h>
#include <softel/core/net/server/MimeTypes.h>

#include "FolderCacheHtml.h"
#include "FileHtml.h"

#include "PluginContainer.h"
#include "BPluginBase.h"
#include "PluginInfo.h"
#include "PluginContainerWebSrvSingleton.h"
#include <softel/core/Ptr.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace SF::CORE::NET;
using namespace SF::CORE;

namespace SF {
namespace WEB {

RequestHandlerHttp::RequestHandlerHttp(const std::string& doc_root) :
      m_doc_root(doc_root) {

}

RequestHandlerHttp::~RequestHandlerHttp() {
   // TODO Auto-generated destructor stub
}

void RequestHandlerHttp::handleRequest(const boost::shared_ptr<RequestBase>& req, boost::shared_ptr<ReplyBase>& rep) {

	boost::shared_ptr<RequestHttpBase> httpReqPtr = boost::static_pointer_cast<RequestHttpBase>(req);

   // Decode url to path.
   std::string request_path;
   if (!ProtoUtils::urlDecode(httpReqPtr->uri, request_path)) {
      rep = ReplyHttpBase::stockReply(ReplyHttpBase::bad_request);
      return;
   }

   // Request path must be absolute and not contain "..".
   if (request_path.empty() || request_path[0] != '/'
         || request_path.find("..") != std::string::npos) {
      rep = ReplyHttpBase::stockReply(ReplyHttpBase::bad_request);
      return;
   }

   // TODO: here adding uri prefix from http request
   string docRootFull = m_doc_root + httpReqPtr->host_based_uri_prefix;

   FolderCacheHtml pathFolder(docRootFull, request_path);

   cout << "pathFolder: " << docRootFull << request_path << endl;
   cout << pathFolder.toString() << endl;

   string extension;

   // Fill out the reply to be sent to the client.
   boost::shared_ptr<ReplyHttpBase> httpRep(new ReplyHttpBase());
   rep = httpRep;
   httpRep->status = ReplyHttpBase::ok;

   // TODO: Make better looking dispatch here
   // TODO: Just for testing Plugins...
   if (request_path.find("/sfplugin/") != string::npos) {

	   cout << "SFPLUGIN Found: " << request_path;

	   if (PluginContainerWebSrvSingleton::getInstance()->run(*httpReqPtr, httpRep->content)) {
		   // Do not send any response headers here...
		   return;
	   } else {
		   rep = ReplyHttpBase::stockReply(ReplyHttpBase::not_found);
		   return;
	   }

   } else
   // Requested Folder
   if (pathFolder.validate()) {
      pathFolder.toHtml(httpRep->content);
      extension = "html";
      cout << "HTML: " << endl << httpRep->content << endl;
   } else {
   // Requested File
	  FileHtml pathFile(docRootFull, *httpReqPtr);
	  cout << pathFile.toString() << endl;

	  extension = pathFile.getExtension();

	  if (!pathFile.setHttpReply(httpRep->content)) {
		  rep = ReplyHttpBase::stockReply(ReplyHttpBase::not_found);
		  return;
	  }

   }

   cout << "MimeType: " << extension << endl;

   httpRep->headers.resize(3);
   httpRep->headers[0].name = "Content-Length";
   httpRep->headers[0].value = boost::lexical_cast <std::string> (httpRep->content.size());
   httpRep->headers[1].name = "Content-Type";
   httpRep->headers[1].value = MimeTypes::getMimeType(extension);
   httpRep->headers[1].name = "Server";
   httpRep->headers[1].value = "WebSrv/2.1 (Softel) (Linux)";
}


} /* namespace WEB */
} /* namespace SF */

