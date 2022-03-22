
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "ServerHttpBase.h"
#include "ServerHttpStandard.h"
#include "ServerHttpCmdBase.h"

int main(int argc, char* argv[])
{
  try
  {
    // Check command line arguments.
    if (argc < 4)
    {
      std::cerr << "Usage: http_server <address> <port> <threads> <doc_root>\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    receiver 0.0.0.0 80 1 .\n";
      std::cerr << "  For IPv6, try:\n";
      std::cerr << "    receiver 0::0 80 1 .\n";
      //return 1;
    }

    // Initialise the server.
    std::size_t num_threads = boost::lexical_cast<std::size_t>(argv[3]);

    //SF::CORE::NET::ServerHttpStandard s(argv[1], argv[2], argv[4], num_threads);
    //SF::CORE::NET::ServerTcpBase s(argv[1], argv[2], num_threads);
    SF::CORE::NET::ServerHttpCmdBase s(argv[1], argv[2], num_threads);

    // Run the server until stopped.
    s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}

