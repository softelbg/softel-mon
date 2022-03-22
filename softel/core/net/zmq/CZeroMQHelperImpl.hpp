// CZeroMQHelperImpl.hpp
#pragma once

#include "IZeroMQHelper.hpp"

namespace SF {
namespace CORE {


class CZeroMQHelperImpl : public IZeroMQHelper
{
public:
    CZeroMQHelperImpl();
    virtual ~CZeroMQHelperImpl();

    HANDLE CreateCtx() const;
    void DestroyCtx(HANDLE ctx) const;
    HANDLE CreateSocket(HANDLE ctx, int type) const;
    void DestroySocket(HANDLE socket) const;

    void Connect(boost::shared_ptr < void > socket, std::string address) const;
    void Bind(boost::shared_ptr < void > socket, std::string address) const;

    void SocketMonitoring(boost::shared_ptr < void > socket, std::string address, int events) const;

    int Poll(::zmq_pollitem_t * itemspoll, int size, long timeout) const;

	void SendMsg(boost::shared_ptr < void > socket, ::zmq_msg_t * zmqmsg, int flags = 0) const;
	bool RecvMsg(boost::shared_ptr < void > socket, ::zmq_msg_t * zmqmsg, int timeout, int & resultsize, int flags = 0) const;
	void RecvMsg(boost::shared_ptr < void > socket, ::zmq_msg_t * zmqmsg, int & resultsize, int flags = 0) const;

	bool HasMoreMessageParts(::zmq_msg_t * zmqmsg) const;

    void RoutingMessage(boost::shared_ptr < void > socketsrc, boost::shared_ptr < void > socketdest, int flagssrc = 0, int flagsdest = 0);

	void CreateMessage(::zmq_msg_t * zmqmsg, int size = 0) const;
	void DestroyMessage(::zmq_msg_t * zmqmsg) const;
	void* GetMessageData(::zmq_msg_t * zmqmsg) const;
    size_t GetMessageSize(::zmq_msg_t * zmqmsg) const;

	void SetSockopt(boost::shared_ptr < void > socket, int name, const void * value, std::size_t len) const;
    void GetSockopt(boost::shared_ptr < void > socket, int name, void * value, std::size_t * len) const;
};

}
}
