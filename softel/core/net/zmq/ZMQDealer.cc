#include <softel/core/common.h>
#include <softel/core/net/zmq/ZMQDealer.hpp>
#include <softel/core/net/zmq/CGuardZMQMsg.hpp>

namespace CN = SF::CORE;

CN::ZMQDealer::ZMQDealer(const std::string& url)
    : ZMQDuplex(ZMQ_DEALER, url)
{
}

CN::ZMQDealer::ZMQDealer(const std::string& url, int hwm)
    : ZMQDuplex(ZMQ_DEALER, url, hwm)
{
}

CN::ZMQDealer::~ZMQDealer()
{
    DLOG << __FUNCTION__ << "::" << __LINE__ << " url = " << url_;
}

void CN::ZMQDealer::poll(const std::function<void(SF::CORE::CGuardZMQMsg & message)>& fnMsgProcessor,
                         int timeoutMS/* = 100 */)
{
    zmq_pollitem_t itemsPoll[] =
    {
        { zmqSocket_.get(), 0, ZMQ_POLLIN, 0 }
    };

    zmqHelper_->Poll(itemsPoll, 1, timeoutMS);

    if (ZMQ_POLLIN & itemsPoll[0].revents) {
        SF::CORE::CGuardZMQMsg recvMsg(zmqHelper_.get());
        int recMsgSize;
        zmqHelper_->RecvMsg(zmqSocket_, recvMsg.Message(), recMsgSize);

        fnMsgProcessor(recvMsg);
    }
}
