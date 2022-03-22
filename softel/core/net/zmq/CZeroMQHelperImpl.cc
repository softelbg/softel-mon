#include <softel/core/common.h>
#include "CZeroMQHelperImpl.hpp"
#include "CGuardZMQMsg.hpp"

namespace CN = SF::CORE;


CN::CZeroMQHelperImpl::CZeroMQHelperImpl()
{

}


CN::CZeroMQHelperImpl::~CZeroMQHelperImpl()
{

}


SF::CORE::HANDLE
CN::CZeroMQHelperImpl::CreateCtx() const
{
    return ::zmq_ctx_new();
}


void
CN::CZeroMQHelperImpl::DestroyCtx(SF::CORE::HANDLE ctx) const
{
    if (! ctx)
    {
		throw std::runtime_error(__FUNCTION__);
    }

    ::zmq_ctx_destroy(ctx);
}


SF::CORE::HANDLE
CN::CZeroMQHelperImpl::CreateSocket(SF::CORE::HANDLE ctx,
                                    int type) const
{
    if (!ctx)
    {
        throw std::runtime_error(__FUNCTION__);
    }

    return ::zmq_socket(ctx, type);
}


void
CN::CZeroMQHelperImpl::DestroySocket(SF::CORE::HANDLE socket) const
{
    if (!socket)
    {
        throw std::runtime_error(__FUNCTION__);
    }

    ::zmq_close(socket);
}


void
CN::CZeroMQHelperImpl::Connect(boost::shared_ptr < void > socket,
                               std::string address) const
{
    if (!socket)
    {
        throw std::runtime_error(__FUNCTION__);
    }

    if (address.empty())
    {
        throw std::runtime_error(__FUNCTION__);
    }

    if (SF::CORE::Fail == ::zmq_connect(socket.get(), address.c_str()))
    {
        throw std::runtime_error(__FUNCTION__);
    }
}


void
CN::CZeroMQHelperImpl::Bind(boost::shared_ptr < void > socket,
                            std::string address) const
{
    if (!socket)
    {
        throw std::runtime_error(__FUNCTION__);
    }

    if (address.empty())
    {
        throw std::runtime_error(__FUNCTION__);
    }

    if (SF::CORE::Fail == ::zmq_bind(socket.get(), address.c_str()))
    {
        throw std::runtime_error(__FUNCTION__);
    }
}


void
CN::CZeroMQHelperImpl::SocketMonitoring(boost::shared_ptr < void > socket,
                                        std::string address,
                                        int events) const
{
    if (!socket)
    {
        throw std::runtime_error(__FUNCTION__);
    }

    if (address.empty())
    {
        throw std::runtime_error(__FUNCTION__);
    }

    if (SF::CORE::Fail ==
            ::zmq_socket_monitor(socket.get(), address.c_str(), events))
    {
        throw std::runtime_error(__FUNCTION__);
    }
}


int
CN::CZeroMQHelperImpl::Poll(::zmq_pollitem_t * itemspoll,
                            int size,
                            long timeout) const
{
    int resultPoll = 0;

    if (!itemspoll)
    {
        throw std::runtime_error(__FUNCTION__);
    }

    if (0 >= size)
    {
        throw std::runtime_error(__FUNCTION__);
    }

    resultPoll = ::zmq_poll(itemspoll, size, timeout);
    if (SF::CORE::Fail == resultPoll)
    {
        throw std::runtime_error(__FUNCTION__);
    }

    return resultPoll;
}


void
CN::CZeroMQHelperImpl::SendMsg(boost::shared_ptr < void > socket,
							   ::zmq_msg_t * zmqmsg,
                               int flags /*= 0*/) const
{
	if (!socket)
	{
		throw std::runtime_error(__FUNCTION__);
	}

	if (!zmqmsg)
	{
		throw std::runtime_error(__FUNCTION__);
	}

	if (SF::CORE::Fail == ::zmq_msg_send(zmqmsg, socket.get(), flags))
	{
		throw std::runtime_error(__FUNCTION__);
	}
}


bool
CN::CZeroMQHelperImpl::RecvMsg(boost::shared_ptr < void > socket,
							   ::zmq_msg_t * zmqmsg,
							   int timeout,
							   int & resultsize,
                               int flags/* = 0*/) const

{
	if (!socket)
	{
		throw std::runtime_error(__FUNCTION__);
	}

	if (!zmqmsg)
	{
		throw std::runtime_error(__FUNCTION__);
	}

	bool result = false;

	::zmq_pollitem_t items[] =
	{
		{ socket.get(), 0, ZMQ_POLLIN, 0 }
	};

	Poll(items, 1, timeout);

	if (items[0].revents & ZMQ_POLLIN)
	{
		resultsize = ::zmq_msg_recv(zmqmsg, socket.get(), flags);
		if (SF::CORE::Fail == resultsize)
		{
			throw std::runtime_error(__FUNCTION__);
		}

		result = true;
	}

	return result;
}

void
CN::CZeroMQHelperImpl::RecvMsg(boost::shared_ptr < void > socket,
								::zmq_msg_t * zmqmsg,
								int & resultsize,
								int flags/* = 0*/) const

{
	if (!socket)
	{
		throw std::runtime_error(__FUNCTION__);
	}

	if (!zmqmsg)
	{
		throw std::runtime_error(__FUNCTION__);
	}

	resultsize = ::zmq_msg_recv(zmqmsg, socket.get(), flags);
	if (SF::CORE::Fail == resultsize)
	{
		throw std::runtime_error(__FUNCTION__);
	}
}

bool
CN::CZeroMQHelperImpl::HasMoreMessageParts(::zmq_msg_t * zmqmsg) const
{
	if (!zmqmsg)
	{
		throw std::runtime_error(__FUNCTION__);
	}

	if (::zmq_msg_more(zmqmsg))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void
CN::CZeroMQHelperImpl::RoutingMessage(boost::shared_ptr < void > socketsrc,
                                      boost::shared_ptr < void > socketdest,
                                      int flagssrc/* = 0*/,
                                      int flagsdest/* = 0*/)
{
    if (!socketsrc)
    {
        throw std::runtime_error(__FUNCTION__);
    }

    if (!socketdest)
    {
        throw std::runtime_error(__FUNCTION__);
    }


	SF::CORE::CGuardZMQMsg zmqMsg(this);
	
	if (SF::CORE::Fail == ::zmq_msg_recv(zmqMsg.Message(), socketsrc.get(), flagssrc))
	{
		throw std::runtime_error(__FUNCTION__);
	}

	if (SF::CORE::Fail == ::zmq_msg_send(zmqMsg.Message(), socketdest.get(), flagsdest))
	{
		throw std::runtime_error(__FUNCTION__);
	}
}


void
CN::CZeroMQHelperImpl::CreateMessage(::zmq_msg_t * zmqmsg,
int size/* = 0*/) const
{
	if (SF::CORE::Fail ==
		((size)
		? ::zmq_msg_init_size(zmqmsg, size)
		: ::zmq_msg_init(zmqmsg)))
	{
		throw std::runtime_error(__FUNCTION__);
	}
}


void
CN::CZeroMQHelperImpl::DestroyMessage(::zmq_msg_t * zmqmsg) const
{
	if (!zmqmsg)
	{
		throw std::runtime_error(__FUNCTION__);
	}

	if (SF::CORE::Fail == ::zmq_msg_close(zmqmsg))
	{
		throw std::runtime_error(__FUNCTION__);
	}
}


void*
CN::CZeroMQHelperImpl::GetMessageData(::zmq_msg_t * zmqmsg) const
{
	if (!zmqmsg)
	{
		throw std::runtime_error(__FUNCTION__);
	}

	return ::zmq_msg_data(zmqmsg);
}

size_t CN::CZeroMQHelperImpl::GetMessageSize(::zmq_msg_t * zmqmsg) const
{
    if (!zmqmsg)
    {
        throw std::runtime_error(__FUNCTION__);
    }

    return ::zmq_msg_size(zmqmsg);
}

void
CN::CZeroMQHelperImpl::SetSockopt(boost::shared_ptr < void > socket,
								  int name,
							      const void * value,
                                  std::size_t len) const
{
	if (!socket)
	{
		throw std::runtime_error(__FUNCTION__);
	}

	if (!value)
	{
		throw std::runtime_error(__FUNCTION__);
	}

	if (SF::CORE::Fail == ::zmq_setsockopt(socket.get(), name, value, len))
	{
		throw std::runtime_error(__FUNCTION__);
	}
}

void CN::CZeroMQHelperImpl::GetSockopt(
                                boost::shared_ptr < void > socket,
                                int name,
                                void * value,
                                std::size_t * len) const
{
    if (!socket)
    {
        throw std::runtime_error(__FUNCTION__);
    }

    if (SF::CORE::Fail == ::zmq_getsockopt(socket.get(), name, value, len))
    {
        throw std::runtime_error(__FUNCTION__);
    }
}
