#pragma once

#include <softel/core/net/zmq/IZeroMQHelper.hpp>
#include <softel/core/net/zmq/CGuardZMQMsg.hpp>
#include <softel/core/net/zmq/IZmqMsgSend.hpp>
#include <softel/core/net/zmq/ZMQBase.h>

namespace SF
{
namespace CORE
{


class ZMQPublish : public ZMQBase, virtual public IZmqMsgSend
{
public:
    explicit ZMQPublish(const std::string& url);
    ZMQPublish(const std::string& url, int hwm);
    virtual ~ZMQPublish();

    virtual void Send(CGuardZMQMsg& msg) const;
    virtual void Send(const void* msg, const int size) const;

    int GetHWM() const;

private:
    void init(const std::string& url, int hwm);
    
    boost::shared_ptr<void> zmqPub_;
};


}
}