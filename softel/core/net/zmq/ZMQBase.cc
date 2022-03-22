#include <softel/core/common.h>


#include <softel/core/net/zmq/ZMQBase.h>
#include <softel/core/net/zmq/IZeroMQHelper.hpp>
#include <softel/core/net/zmq/CZeroMQHelperImpl.hpp>

namespace CN = SF::CORE;

std::shared_ptr<void> CN::ZMQBase::zmqContext_;
std::shared_ptr<CN::IZeroMQHelper> CN::ZMQBase::zmqHelper_;

CN::ZMQBase::ZMQBase(const std::string& url)
: url_(url)
{
    init();
}

CN::ZMQBase::~ZMQBase()
{
    DLOGF << std::endl;
    TRY(zmqHelper_->DestroySocket(zmqSocket_.get()));
}

void CN::ZMQBase::init()
{
    DLOGF << "url = " << url_ << std::endl;

    if (!zmqHelper_) {
        zmqHelper_.reset(new CZeroMQHelperImpl());
    }

    if (!zmqContext_) {
        zmqContext_.reset(zmqHelper_->CreateCtx());
    }
}

void CN::ZMQBase::destroy()
{
    TRY(zmqHelper_->DestroyCtx(zmqContext_.get());
        zmqContext_.reset();
        zmqHelper_.reset();
    );
}

void CN::ZMQBase::connect()
{
    DLOGF << "url = " << url_;
    zmqHelper_->Connect(zmqSocket_, url_);
}

void CN::ZMQBase::bind()
{
    DLOGF << "url = " << url_;
    zmqHelper_->Bind(zmqSocket_, url_);
}

int CN::ZMQBase::getRecvMore() const
{
    int recvmore = 0;
    size_t len = sizeof(recvmore);
    zmqHelper_->GetSockopt(zmqSocket_, ZMQ_RCVMORE, &recvmore, &len);

    return recvmore;
}
