
#include <Wt/WApplication>
#include <Wt/WBreak>

#include "AdminPanel.h"
#include "MLMachine.h"

using namespace Wt;
using namespace SF::WT;

WApplication *createApplication(const WEnvironment& env)
{
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
	return new AdminPanel(env);
}

int main(int argc, char **argv)
{
    SF::ML::Machines::m.reset(new SF::ML::MLMachine());
    
    return WRun(argc, argv, &createApplication);
}
