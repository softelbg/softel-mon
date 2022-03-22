#pragma once

#include <softel/core/common.h>
#include "IZeroMQHelper.hpp"


namespace SF
{
namespace CORE
{


class CGuardZMQMsg
	: private boost::noncopyable
{
public:
	CGuardZMQMsg(SF::CORE::IZeroMQHelper * zmqhelper,
				 int size = CGuardZMQMsg::IsRecvMsg)
		: 
		//logClient_(boost::log::keywords::channel = "CGuardZMQMsg") ,
		zmqHelper_(zmqhelper),
		zmqMessage_(NULL),
		size_ (size)
	{
	}


	~CGuardZMQMsg()
	{
		try
		{
			if (zmqMessage_)
			{
				zmqHelper_->DestroyMessage(zmqMessage_);
			}
		}
		catch (std::exception & exc)
		{
			ELOGF;
		}
		catch (...)
		{
			ELOGF << "Dtor: Undefined exception";
		}
	}

	void*
	Data()
	{
		return zmqHelper_->GetMessageData(Message());
	}

    size_t DataSize()
    {
        return zmqHelper_->GetMessageSize(Message());
    }

	::zmq_msg_t *
	Message()
	{
		if (!zmqMessage_)
		{
			zmqHelper_->CreateMessage(&zmqMsg_, size_);
			zmqMessage_ = &zmqMsg_;
		}

		return zmqMessage_;
	}

private:
	static int const IsRecvMsg =0;

	//SF::NSLogger::CLoggerImpl::TLogClient logClient_;

	SF::CORE::IZeroMQHelper * zmqHelper_;

	::zmq_msg_t zmqMsg_;
	::zmq_msg_t * zmqMessage_;
	int size_;
};


} // CORE
} //SF