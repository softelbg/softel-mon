#pragma once

#include <softel/core/net/zmq/ZMQBase.h>

namespace SF
{
namespace CORE
{

class CGuardZMQMsg;

class ZMQDuplex : public ZMQBase
{
public:
    virtual ~ZMQDuplex();
        
    void recv(std::function<void(CGuardZMQMsg& msg, const int size)> fnRecv) const;
    void recvMore(std::function<void(CGuardZMQMsg& msg, int idx)> fnRecv) const;
    
    void send(CGuardZMQMsg& zmqMsg) const;
    void sendMore(CGuardZMQMsg& zmqMsg) const;
    void send(const void* msg, size_t size) const;

    void routeMore(const ZMQDuplex& dest) const;

protected:
    ZMQDuplex(int type, const std::string& url);
    ZMQDuplex(int type, const std::string& url, int hwm);

    void init();

    int hwm_;
    int typeSocket_;
};

class ZMQPair : public ZMQDuplex
{
public:
    explicit ZMQPair(const std::string& url) : ZMQDuplex(ZMQ_PAIR, url) {}
    virtual ~ZMQPair() {}

};

// PUSH / PULL
class ZMQPush : public ZMQDuplex
{
public:
    explicit ZMQPush(const std::string& url) : ZMQDuplex(ZMQ_PUSH, url) {}
    virtual ~ZMQPush() {}

};

class ZMQPull : public ZMQDuplex
{
public:
    explicit ZMQPull(const std::string& url) : ZMQDuplex(ZMQ_PULL, url) {}
    virtual ~ZMQPull() {}

};

// REQ / REP
class ZMQReq : public ZMQDuplex
{
public:
    explicit ZMQReq(const std::string& url) : ZMQDuplex(ZMQ_REQ, url) {}
    virtual ~ZMQReq() {}

};

class ZMQRep : public ZMQDuplex
{
public:
    explicit ZMQRep(const std::string& url) : ZMQDuplex(ZMQ_REP, url) {}
    virtual ~ZMQRep() {}

};



}
}
