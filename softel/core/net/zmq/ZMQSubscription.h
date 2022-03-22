#pragma once

#include "ZMQBase.h"

namespace SF
{
namespace CORE
{


class CGuardZMQMsg;


class ZMQSubscription : public ZMQBase
{
public:
    ZMQSubscription(const std::string& url, const void * filter = "", size_t sizeFilter = 0);
    virtual ~ZMQSubscription();

    bool Receive(long timeout, std::function<void(CGuardZMQMsg& msg, const int size)> f);
    void Receive(std::function<void(CGuardZMQMsg& msg, const int size)> f);
    void ReceiveLoop(std::function<void(CGuardZMQMsg& msg, const int size)> f);

    void Subscribe(const void * filter, size_t sizeFilter);

private:
    boost::shared_ptr<void> zmqSub_;
};


}
}
