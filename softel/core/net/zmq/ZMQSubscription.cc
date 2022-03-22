#include <softel/core/common.h>

#include <softel/core/net/zmq/ZMQSubscription.h>
#include <softel/core/net/zmq/CGuardZMQMsg.hpp>


namespace CN = SF::CORE;

CN::ZMQSubscription::ZMQSubscription(const std::string& url, const void * filter /*= ""*/, size_t sizeFilter /*= 0*/)
: ZMQBase(url)
{
    zmqSub_.reset(zmqHelper_->CreateSocket(zmqContext_.get(), ZMQ_SUB));
    zmqHelper_->SetSockopt(zmqSub_, ZMQ_SUBSCRIBE, filter, sizeFilter);
    zmqHelper_->Connect(zmqSub_, url);
}

CN::ZMQSubscription::~ZMQSubscription() {
}

void CN::ZMQSubscription::ReceiveLoop(std::function<void(CGuardZMQMsg& msg, const int size)> fnreceive) {
    while (true) {
        Receive(fnreceive);
    }
}

bool CN::ZMQSubscription::Receive(long timeout, std::function<void(CGuardZMQMsg& msg, const int size)> fnreceive) {
    int sizeResult = 0;
    CGuardZMQMsg msg(zmqHelper_.get());

    bool result = false;

    if (zmqHelper_->RecvMsg(zmqSub_, msg.Message(), timeout, sizeResult, 0)) {
        fnreceive(msg, sizeResult);
    } else {
        result = true;
    }

    return result;
}

void CN::ZMQSubscription::Receive(std::function<void(CGuardZMQMsg& msg, const int size)> fnreceive) {
    Receive(-1, fnreceive);
}

void CN::ZMQSubscription::Subscribe(const void * filter, size_t sizeFilter) {
    zmqHelper_->SetSockopt(zmqSub_, ZMQ_SUBSCRIBE, filter, sizeFilter);
}
