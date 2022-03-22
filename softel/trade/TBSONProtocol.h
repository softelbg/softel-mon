/*
 * TBSONProtocol.h
 *
 *  Created on: May 13, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_TRADE_TBSONPROTOCOL_H_
#define SOFTEL_TRADE_TBSONPROTOCOL_H_

#include <softel/core/common.h>
#include <softel/core/commonDefs.h>
#include <thrift/protocol/TProtocol.h>

namespace SF {
namespace TRADE {

struct BSONObj {
    enum Types {
        DOCUMENT,
        ARRAY
    };

    template<class T>
    BSONObj& operator<<(const T& val) {
        DLOGF << std::endl;
        append(val);
        return *this;
    }
    template<class T>
    void append(const std::string& key, const T& val) {
        DLOGF << std::endl;
        doc.append(bsoncxx::builder::basic::kvp(key, val));
    }
    template<class T>
    void append(const T& val) {
        DLOGF << std::endl;
        switch(type) {
            case BSONObj::Types::ARRAY :
                arr.append(val);
                break;

            case BSONObj::Types::DOCUMENT :
                doc.append(bsoncxx::builder::basic::kvp("val", val));
                break;

            default :
                break;
        }
    }

    BSONObj(const std::string& n, Types t) : name(n), type(t)  {}

    std::string name;
    Types type; // TODO: Type based polymorph here...

    bsoncxx::builder::basic::document doc{};
    bsoncxx::builder::basic::array arr{};
};

class TBSONProtocol : public virtual ::apache::thrift::protocol::TProtocol {
public:
    TBSONProtocol();
    virtual ~TBSONProtocol();

    void pushDocument(const std::string& name);
    void pushArray(const std::string& name);
    void popDocument();
    void popArray();

    /**
     * Writing functions.
     */

    uint32_t writeMessageBegin_virt(const std::string& name,
            const ::apache::thrift::protocol::TMessageType messageType,
            const int32_t seqid);
    uint32_t writeMessageEnd_virt();
    uint32_t writeStructBegin_virt(const char* name);
    uint32_t writeStructEnd_virt();
    uint32_t writeFieldBegin_virt(const char* name,
            const ::apache::thrift::protocol::TType fieldType,
            const int16_t fieldId);
    uint32_t writeFieldEnd_virt();
    uint32_t writeFieldStop_virt();
    uint32_t writeMapBegin_virt(const ::apache::thrift::protocol::TType keyType,
            const ::apache::thrift::protocol::TType valType, const uint32_t size);
    uint32_t writeMapEnd_virt();
    uint32_t writeListBegin_virt(const ::apache::thrift::protocol::TType elemType, const uint32_t size);
    uint32_t writeListEnd_virt();
    uint32_t writeSetBegin_virt(const ::apache::thrift::protocol::TType elemType, const uint32_t size);
    uint32_t writeSetEnd_virt();
    uint32_t writeBool_virt(const bool value);
    uint32_t writeByte_virt(const int8_t byte);
    uint32_t writeI16_virt(const int16_t i16);
    uint32_t writeI32_virt(const int32_t i32);
    uint32_t writeI64_virt(const int64_t i64);
    uint32_t writeDouble_virt(const double dub);
    uint32_t writeString_virt(const std::string& str);
    uint32_t writeBinary_virt(const std::string& str);


    /**
     * Reading functions
     */

    virtual uint32_t readMessageBegin_virt(std::string& name,
            ::apache::thrift::protocol::TMessageType& messageType,
            int32_t& seqid);
    virtual uint32_t readMessageEnd_virt();
    virtual uint32_t readStructBegin_virt(std::string& name);
    virtual uint32_t readStructEnd_virt();
    virtual uint32_t readFieldBegin_virt(std::string& name, ::apache::thrift::protocol::TType& fieldType, int16_t& fieldId);
    virtual uint32_t readFieldEnd_virt();
    virtual uint32_t readMapBegin_virt(::apache::thrift::protocol::TType& keyType,
            ::apache::thrift::protocol::TType& valType, uint32_t& size);
    virtual uint32_t readMapEnd_virt();
    virtual uint32_t readListBegin_virt(::apache::thrift::protocol::TType& elemType, uint32_t& size);
    virtual uint32_t readListEnd_virt();
    virtual uint32_t readSetBegin_virt(::apache::thrift::protocol::TType& elemType, uint32_t& size);
    virtual uint32_t readSetEnd_virt();
    virtual uint32_t readBool_virt(bool& value);
    virtual uint32_t readBool_virt(std::vector<bool>::reference value);
    virtual uint32_t readByte_virt(int8_t& byte);
    virtual uint32_t readI16_virt(int16_t& i16);
    virtual uint32_t readI32_virt(int32_t& i32);
    virtual uint32_t readI64_virt(int64_t& i64);
    virtual uint32_t readDouble_virt(double& dub);
    virtual uint32_t readString_virt(std::string& str);
    virtual uint32_t readBinary_virt(std::string& str);


private:
    std::stack<BSONObj> obj_;
};

} /* namespace TRADE */
} /* namespace SF */

#endif /* SOFTEL_TRADE_TBSONPROTOCOL_H_ */
