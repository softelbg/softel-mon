
#include <softel/core/common.h>
#include <softel/core/util/Options.h>
#include <softel/core/util/OptionParam.hpp>
#include <softel/trade/idl/Protobuf.h>
#include <softel/trade/TradeMachine.h>

int main(int argc, char **argv)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    SF::CORE::UTIL::Options opt(argc, argv, "=");
    SF::TRADE::TradeMachine::start(opt);

    return 0;
}
