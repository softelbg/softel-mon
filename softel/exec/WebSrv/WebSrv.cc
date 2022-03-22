/*
 * HttpCommSrv.cc
 *
 *  Created on: 25.01.2010
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include <softel/core/net/web/ServerHttpWeb.h>
#include "DemonizeWebSrv.h"

#include <softel/core/net/server/ServerHttpBase.h>
#include <softel/core/net/server/ServerHttpStandard.h>
#include <softel/core/net/server/ServerHttpCmdBase.h>
#include <softel/core/net/web/PluginContainerWebSrvSingleton.h>

#include <softel/core/error/CErrorCommon.h>
#include <softel/core/error/CErrorSql.h>
#include <softel/core/system/CSystemTools.h>
#include <softel/core/Ptr.h>

using namespace std;
using namespace SF::CORE::SYSTEM;
using namespace SF::CORE::ERROR;
using namespace SF::WEB;

int main(int argc, char* argv[])
{

   // Check command line arguments.
   if (argc < 4) {
      std::cerr << "Usage: " << argv[0] << " <address> <port> <threads> <doc_root>\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    receiver 0.0.0.0 80 1 .\n";
      std::cerr << "  For IPv6, try:\n";
      std::cerr << "    receiver 0::0 80 1 .\n";
      return 1;
   }

   std::size_t num_threads = boost::lexical_cast<std::size_t>(argv[3]);

   int pidMainProcess = 0;
   int pidChildProcess = 0;
   int pidWaitedChildProcess = 0;
   int childStatus = 0;

   CSystemTools::doInitialFork();
   
   try {

      pidMainProcess = CSystemTools::doGetPid();
      cout << "Main Process PID: " << pidMainProcess << endl;
      
      while (true) {

         pidChildProcess = CSystemTools::doFork();
         // Child Process should start Http Server
         if (pidChildProcess == 0) {

        	PluginContainerWebSrvSingleton::init();
            ServerHttpWeb s(argv[1], argv[2], argv[4], num_threads);
            s.run();

            break;
         }

         // Main Process
         pidWaitedChildProcess = CSystemTools::doWait(&childStatus);

         cout << "Main Process: Child:" << pidWaitedChildProcess << " Exited with status:" << childStatus << endl;

         CSystemTools::doSleep(1);
      }
      
  } catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << "\n";
  } catch (CErrorSql& error) {
      //LOGHCS(LOG_LEVEL_ERROR, error.getMessage());
      std::cerr << "exception: " << error.getMessage() << endl;
   } catch (CErrorCommon& error) {
      //LOGHCS(LOG_LEVEL_ERROR, error.getMessage());
      std::cerr << "exception: " << error.getMessage() << endl;
   }

  return 0;
}

