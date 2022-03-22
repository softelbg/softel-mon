#pragma once

#include <softel/core/net/zmq/CGuardZMQMsg.hpp>

namespace SF
{
namespace CORE
{


class IZmqMsgSend
{
public:
    virtual ~IZmqMsgSend() {}

    virtual void Send(const void* msg, const int size) const = 0;
    virtual void Send(CGuardZMQMsg& msg) const = 0;

    virtual int GetHWM() const = 0;
};


}
}