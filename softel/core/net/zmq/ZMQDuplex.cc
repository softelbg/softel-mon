#include <softel/core/common.h>
#include <softel/core/net/zmq/ZMQDuplex.hpp>
#include <softel/core/net/zmq/CGuardZMQMsg.hpp>


namespace CN = SF::CORE;

CN::ZMQDuplex::ZMQDuplex(int type, const std::string& url)
    : typeSocket_(type), ZMQBase(url), hwm_(1000)
{
    init();
}

CN::ZMQDuplex::ZMQDuplex(int type, const std::string& url, int hwm)
    : typeSocket_(type), ZMQBase(url), hwm_(hwm)
{
    init();
}

void CN::ZMQDuplex::init()
{
    DLOGF << " url = " << url_ << " hwm = " << hwm_ << std::endl;

    zmqSocket_.reset(zmqHelper_->CreateSocket(zmqContext_.get(), typeSocket_));

    // Set HWM
    zmqHelper_->SetSockopt(zmqSocket_, ZMQ_SNDHWM, &hwm_, sizeof(hwm_));

    // Set Linger
    int linger = 0;
    zmqHelper_->SetSockopt(zmqSocket_, ZMQ_LINGER, &linger, sizeof(linger));
}

CN::ZMQDuplex::~ZMQDuplex()
{
    DLOGF << " url = " << url_ << std::endl;
}

void CN::ZMQDuplex::recv(std::function<void(CGuardZMQMsg& msg, const int size)> fnRecv) const
{
    int sizeResult = 0;
    CGuardZMQMsg recvMsg(zmqHelper_.get());

    zmqHelper_->RecvMsg(zmqSocket_, recvMsg.Message(), sizeResult);

    fnRecv(recvMsg, sizeResult);
}

void CN::ZMQDuplex::recvMore(std::function<void(CGuardZMQMsg& msg, int idx)> fnRecv) const
{
    int idx = 0;

    while (true) {
        int sizeResult = 0;
        CGuardZMQMsg recvMsg(zmqHelper_.get());
        zmqHelper_->RecvMsg(zmqSocket_, recvMsg.Message(), sizeResult);

        fnRecv(recvMsg, idx);

        if (!getRecvMore()) {
            break;
        }

        ++idx;
    }
}

void CN::ZMQDuplex::send(CGuardZMQMsg& zmqMsg) const
{
    zmqHelper_->SendMsg(zmqSocket_, zmqMsg.Message());
}

void CN::ZMQDuplex::sendMore(CGuardZMQMsg& zmqMsg) const
{
    zmqHelper_->SendMsg(zmqSocket_, zmqMsg.Message(), ZMQ_SNDMORE);
}

void CN::ZMQDuplex::send(const void* msg, size_t size) const
{
    CGuardZMQMsg zmqMsg(zmqHelper_.get(), size);
    memcpy(zmqMsg.Data(), msg, size);
    zmqHelper_->SendMsg(zmqSocket_, zmqMsg.Message());
}

void CN::ZMQDuplex::routeMore(const ZMQDuplex& dest) const
{
    while (true) {
        int sizeResult = 0;
        CGuardZMQMsg recvMsg(zmqHelper_.get());

        zmqHelper_->RecvMsg(zmqSocket_, recvMsg.Message(), sizeResult);

        int rcvmore = 0;
        size_t len = sizeof(rcvmore);
        zmqHelper_->GetSockopt(zmqSocket_, ZMQ_RCVMORE, &rcvmore, &len);
        
        if (rcvmore) {
            dest.sendMore(recvMsg);
        }
        else {
            dest.send(recvMsg);
            break;
        }
    }
}
