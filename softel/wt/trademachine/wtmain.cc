
#include <Wt/WApplication>
#include <Wt/WBreak>

#include "MainPanel.h"

#include <softel/core/common.h>
#include <softel/core/util/Options.h>
#include <softel/core/util/OptionParam.hpp>
#include <softel/trade/idl/Protobuf.h>

#include <softel/wt/common.h>
#include <softel/wt/TradeMachine.h>
#include <softel/wt/ForexMachine.h>
#include <softel/wt/ItemTradeMachine.h>
#include <softel/wt/TallentMachine.h>
#include <softel/wt/BitcoinMachine.h>

using namespace Wt;
using namespace SF::WT;

WApplication *createApplication(const WEnvironment& env)
{
    /*
     * You could read information from the environment to decide whether
     * the user has permission to start a new application
     */
    return new MainPanel(env);
}

int main(int argc, char **argv)
{
    SF::CORE::UTIL::Options opt(argc, argv, "=");

    SF::TRADE::deserialize(opt, Machines::tm);
    if (Machines::tm.type == TRADING_WIDGET) {
        Machines::m.reset(new TradeMachine(Machines::tm));
    } else if (Machines::tm.type == FOREX_WIDGET) {
        Machines::m.reset(new ForexMachine(Machines::tm));
    } else if (Machines::tm.type == ITEM_TRADE_WIDGET) {
        Machines::m.reset(new ItemTradeMachine(Machines::tm));
    } else if (Machines::tm.type == TALLENT_TRADE_WIDGET) {
        Machines::m.reset(new TallentMachine(Machines::tm));
    } else if (Machines::tm.type == BITCOIN_TRADE_WIDGET) {
        Machines::m.reset(new BitcoinMachine(Machines::tm));
    }

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    return WRun(argc, argv, &createApplication);
}
