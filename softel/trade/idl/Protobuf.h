/*
 * protobuf.h
 *
 *  Created on: Apr 21, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_TRADE_IDL_PROTOBUF_H_
#define SOFTEL_TRADE_IDL_PROTOBUF_H_

#include <softel/core/net/zmq/ZMQDuplex.hpp>
#include <softel/core/net/zmq/CGuardZMQMsg.hpp>

#include <softel/trade/idl/messages.pb.h>

namespace SF {
namespace TRADE {

template<class T>
struct ProtoRequest {
    ProtoRequest(EnMsgTypeProto t) : type(t) {};
    ProtoRequest(EnMsgTypeProto t, const T& m) : type(t), msg(m) {};

    EnMsgTypeProto type;
    T msg;
};

template<class T>
struct ProtoResponse {
    ProtoResponse(EnMsgTypeProto t) : type(t) {};

    EnMsgTypeProto type;
    T msg;
};

struct Protobuf {

    static void sendBaseMessageProto(SF::CORE::ZMQDuplex& sock, EnMsgTypeProto type) {
        DLOGF << std::endl;
        BaseMessageProto bm;
        bm.set_msgtype(type);
        size_t sizeResponse = bm.ByteSize();
        SF::CORE::CGuardZMQMsg zmqMsgType(sock.getHelper().get(), sizeResponse);
        bm.SerializeToArray(zmqMsgType.Data(), sizeResponse);
        sock.sendMore(zmqMsgType);
    }

    template<class T> static void sendMsgProto(SF::CORE::ZMQDuplex& sock, T& msg) {
        DLOGF << std::endl;
        size_t msgSize = msg.ByteSize();
        SF::CORE::CGuardZMQMsg zmqMsg(sock.getHelper().get(), msgSize);
        msg.SerializeToArray(zmqMsg.Data(), msgSize);
        sock.send(zmqMsg);
    }
    template<class T> static void sendMsgProto(SF::CORE::ZMQDuplex& sock, EnMsgTypeProto type, T& msg) {
        DLOGF << std::endl;
        sendBaseMessageProto(sock, type);
        sendMsgProto(sock, msg);
    }

    template<class TREQ, class TRES>
    static void sendMsgProto(SF::CORE::ZMQDuplex& sock, ProtoRequest<TREQ>& req, ProtoResponse<TRES>& res) {
        DLOGF << std::endl;

        sendBaseMessageProto(sock, req.type);
        sendMsgProto(sock, req.msg);

        DLOGF << std::endl;

        BaseMessageProto bmIn;

        sock.recvMore([&](SF::CORE::CGuardZMQMsg& msg, int idx) {
            DLOGF << std::endl;
            switch (idx) {
                case 0 :
                    bmIn.ParseFromArray(msg.Data(), msg.DataSize());
                    res.type = bmIn.msgtype();
                    DLOG << "MSG Type = " << bmIn.msgtype();
                    break;

                    // TODO: ASSERT the type is already pointed in TRES...
                case 1 :
                    if (bmIn.msgtype() == res.type) {
                        res.msg.ParseFromArray(msg.Data(), msg.DataSize());
                    }
                    break;

                default:
                    ELOGF << "Unknown idx = " << idx << std::endl;
            }
        });

        DLOGF << std::endl;
    }
    template<class TREQ>
    static void sendMsgProto(SF::CORE::ZMQDuplex& sock, ProtoRequest<TREQ>& req) {
        DLOGF << std::endl;
        ProtoResponse<EmptyMessageProto> res(EnMsgTypeProto::EmptyMessageProtoResponse);
        sendMsgProto(sock, req, res);
    }

};


} /* namespace TRADE */
} /* namespace SF */

#endif /* SOFTEL_TRADE_IDL_PROTOBUF_H_ */
