
#include <softel/core/common.h>
#include <softel/core/util/Options.h>
#include <softel/core/util/OptionParam.hpp>
#include <softel/trade/idl/messages.pb.h>

#include <softel/core/intf/IModule.hpp>
#include "ModuleMsgRate.hpp"
#include "ModulePublish.hpp"
#include "ModuleMsgMD.hpp"
#include "ModuleTrade.h"
#include "ModuleMsgRaw.hpp"


#define ZMQSUB_VER  1.6

//static NSCommon::NSLogger::CLoggerImpl::TLogClient logClient_(boost::log::keywords::channel = "ZMQSub");

void initModules(std::map<std::string, std::shared_ptr<SF::CORE::IModule> >& modules, const SF::CORE::UTIL::Options& opt)
{
    std::vector<std::shared_ptr<SF::CORE::IModule> > arrModules;

    arrModules.push_back(std::shared_ptr<SF::CORE::IModule>(new SF::ModuleMsgRate(opt)));
    arrModules.push_back(std::shared_ptr<SF::CORE::IModule>(new SF::ModulePublish(opt)));
    arrModules.push_back(std::shared_ptr<SF::CORE::IModule>(new SF::ModuleMsgMD(opt)));
    arrModules.push_back(std::shared_ptr<SF::CORE::IModule>(new SF::ModuleTrade(opt)));
    arrModules.push_back(std::shared_ptr<SF::CORE::IModule>(new SF::ModuleMsgRaw(opt)));

    for (size_t i = 0; i < arrModules.size(); ++i) {
        modules[arrModules[i]->getName()] = arrModules[i];
    }
}

int main(int argc, char* argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    SF::CORE::UTIL::Options opt(argc, argv, "=");

    std::map<std::string, std::shared_ptr<SF::CORE::IModule> > modules;
    initModules(modules, opt);

    // Help
    if (opt.has("-h")) {
        std::cout << std::endl << "Usage with any of the modules and their options: " << opt["exe"];
        std::cout << " module=ModuleDefault -h -f" << std::endl << std::endl;

        for (auto it = modules.cbegin(); it != modules.cend(); ++it) {
            std::cout << it->first << " [" << it->second->getInfo() << "]" << std::endl;
            for (size_t i = 0; i < it->second->params().size(); ++i) {
                const auto params = it->second->params();
                std::cout << "\t\t\t" << params[i]->toString() << std::endl;
            }
            std::cout << std::endl;
        }

        std::cout << std::endl << "version " << ZMQSUB_VER << " built at " << __DATE__ << " : " << __TIME__ << std::endl;

        std::cout << boost::posix_time::second_clock::local_time() << std::endl;

        exit(0);
    }

    // boost logger output to file
    if (opt.has("-f")) {
        /*    NSCommon::NSLogger::CLoggerImpl::Create(
            SF::CORE::UTIL::Options::getBasePath(), SF::CORE::UTIL::Options::getExecutableName());
         */
    }

    // Execute various modules.
    SF::CORE::UTIL::OptionParam<std::string> paramModule(opt, "module", "ModuleDefault");

    auto itModule = modules.find(paramModule.getValue());
    if (itModule != modules.cend()) {
        TRY(itModule->second->run());
    } 

    SF::CORE::ZMQBase::destroy();

    return 0;
}

