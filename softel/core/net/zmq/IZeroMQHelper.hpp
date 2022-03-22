// IZeroMQHelper.hpp
#pragma once

namespace SF {
namespace CORE {

typedef void * HANDLE;

static const int Fail = -1;

struct IZeroMQHelper
{
    IZeroMQHelper()
    {

    }

    virtual ~IZeroMQHelper()
    {
    }

    virtual HANDLE CreateCtx() const = 0;
    virtual void DestroyCtx(HANDLE ctx) const = 0;
    virtual HANDLE CreateSocket(HANDLE ctx, int type) const = 0;
    virtual void DestroySocket(HANDLE socket) const = 0;

    virtual void Connect(boost::shared_ptr < void > socket, std::string address) const = 0;
    virtual void Bind(boost::shared_ptr < void > socket, std::string address) const = 0;

    virtual void SocketMonitoring(boost::shared_ptr < void > socket, std::string address, int events) const = 0;

    virtual int Poll(::zmq_pollitem_t * itemspoll, int size, long timeout) const = 0;

	virtual void SendMsg(boost::shared_ptr < void > socket, ::zmq_msg_t * zmqmsg, int flags = 0) const = 0;

	virtual bool RecvMsg(boost::shared_ptr < void > socket,
			::zmq_msg_t * zmqmsg,
			int timeout,
			int & resultsize,
            int flags = 0) const = 0;

	virtual void RecvMsg(boost::shared_ptr < void > socket,
			::zmq_msg_t * zmqmsg,
			int & resultsize,
			int flags = 0) const = 0;

	virtual bool HasMoreMessageParts(::zmq_msg_t * zmqmsg) const = 0;

    virtual void RoutingMessage(boost::shared_ptr < void > socketsrc,
                   boost::shared_ptr < void > socketdest,
                   int flagssrc = 0,
                   int flagsdest = 0) = 0;

	virtual void CreateMessage(::zmq_msg_t * zmqmsg, int size = 0) const = 0;
	virtual void DestroyMessage(::zmq_msg_t * zmqmsg) const = 0;

	virtual void* GetMessageData(::zmq_msg_t * zmqmsg) const = 0;
    virtual size_t GetMessageSize(::zmq_msg_t * zmqmsg) const = 0;

	virtual void SetSockopt(boost::shared_ptr < void > socket,
			   int name,
			   const void * value,
			   std::size_t len) const = 0;

    virtual void GetSockopt(
        boost::shared_ptr < void > socket,
        int name,
        void * value,
        std::size_t * len) const = 0;
};

} // CORE
} //SF
