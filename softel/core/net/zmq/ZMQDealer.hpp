#pragma once
#include <softel/core/net/zmq/ZMQDuplex.hpp>

namespace SF
{
namespace CORE
{

class CGuardZMQMsg;

class ZMQDealer : public ZMQDuplex
{
public:
    explicit ZMQDealer(const std::string& url);
    ZMQDealer(const std::string& url, int hwm);
    virtual ~ZMQDealer();

    void poll(const std::function<void(SF::CORE::CGuardZMQMsg & message)>& fnMsgProcessor, int timeoutMS = 100);
};


}
}