#include <softel/core/common.h>

#include <softel/core/net/zmq/ZMQPublish.h>
#include <softel/core/net/zmq/CZeroMQHelperImpl.hpp>

namespace CN = SF::CORE;

CN::ZMQPublish::ZMQPublish(const std::string& url)
    : ZMQBase(url)
{
    init(url, 10000);
}

CN::ZMQPublish::ZMQPublish(const std::string& url, int hwm)
    : ZMQBase(url)
{
    init(url, hwm);
}

CN::ZMQPublish::~ZMQPublish()
{
}

void CN::ZMQPublish::init(const std::string& url, int hwm)
{
    zmqPub_.reset(zmqHelper_->CreateSocket(zmqContext_.get(), ZMQ_PUB));

    // Set HWM
    zmqHelper_->SetSockopt(zmqPub_, ZMQ_SNDHWM, &hwm, sizeof(hwm));

    // Set Linger
    int linger = 0;
    zmqHelper_->SetSockopt(zmqPub_, ZMQ_LINGER, &linger, sizeof(linger));

    zmqHelper_->Bind(zmqPub_, url);
}

void CN::ZMQPublish::Send(const void* msg, const int size) const
{
    CGuardZMQMsg zmqMsg(zmqHelper_.get(), size);
    memcpy(zmqMsg.Data(), msg, size);
    zmqHelper_->SendMsg(zmqPub_, zmqMsg.Message());
}

void CN::ZMQPublish::Send(CGuardZMQMsg& msg) const
{
    zmqHelper_->SendMsg(zmqPub_, msg.Message());
}

int CN::ZMQPublish::GetHWM() const
{
    int hwm = 0;
    size_t len = sizeof(hwm);

    zmqHelper_->GetSockopt(zmqPub_, ZMQ_SNDHWM, &hwm, &len);

    return hwm;
}
