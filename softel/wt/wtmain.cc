
#include <Wt/WApplication>
#include <Wt/WBreak>

#include "TradingApplication.h"

using namespace Wt;
using namespace SF::WT;

WApplication *createApplication(const WEnvironment& env)
{
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
  //return new EscrowApplication(env);
	return new TradingApplication(env);
}

int main(int argc, char **argv)
{
    return WRun(argc, argv, &createApplication);
}
