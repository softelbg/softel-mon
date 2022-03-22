/*
 * TJSONProtocol.cc
 *
 *  Created on: May 18, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include <softel/trade/TJSONProtocol.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;

namespace SF {
namespace TRADE {

TJSONProtocol::TJSONProtocol()
: TProtocol(NULL) {
    // TODO Auto-generated constructor stub

}

TJSONProtocol::~TJSONProtocol() {
    // TODO Auto-generated destructor stub
}

void TJSONProtocol::delim(std::stack<std::string>& s) {
    if (!s.empty()) {
        json_ << s.top();
        s.pop();
    }
}
void TJSONProtocol::delim() {
    delim(delim_);
}

uint32_t TJSONProtocol::writeMessageBegin_virt(const std::string& name,
        const ::apache::thrift::protocol::TMessageType messageType,
        const int32_t seqid) {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::writeMessageEnd_virt() {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::writeStructBegin_virt(const char* name) {
    if (!end_.empty() && (end_.top() == "}" || end_.top() == "]") ) {
        json_ << ", ";
    }

    json_ << "{ \"" << name << "\" : { " << std::endl;
    end_.push("{");
    return 0;
}

uint32_t TJSONProtocol::writeStructEnd_virt() {
    json_ << " } } ";
    end_.push("}");
    return 0;
}

uint32_t TJSONProtocol::writeFieldBegin_virt(const char* name,
        const ::apache::thrift::protocol::TType fieldType,
        const int16_t fieldId) {

    json_ << "\"" << name << "\" : { \"tid\" : \"" << fieldId << "\", \"type\" : \"" << fieldType << "\", \"value\" : ";
    delimField_.push(", ");
    end_.push(":");

    return 0;
}

uint32_t TJSONProtocol::writeFieldEnd_virt() {

    json_ << " } ";
    delim(delimField_);
    end_.push("}");

    return 0;
}

uint32_t TJSONProtocol::writeFieldStop_virt() {
    //DLOGF << std::endl;
    json_.seekp(-2, json_.cur);
    return 0;
}

uint32_t TJSONProtocol::writeMapBegin_virt(
        const ::apache::thrift::protocol::TType keyType,
        const ::apache::thrift::protocol::TType valType, const uint32_t size) {

    json_ << " { \"map\" : { \"keyType\" : \"" << keyType << "\", \"valType\" : \"";
    json_ << valType << "\", \"size\" : \"" << size << "\", \"value\" : { ";

    if (size > 0) {
        delim_.push(" : ");
        for (int i = 0; i < size - 1; i++) {
            delim_.push(", ");
            delim_.push(" : ");
        }
    }

    end_.push("{");

    return 0;
}

uint32_t TJSONProtocol::writeMapEnd_virt() {

    json_ << "} } }";
    end_.push("}");

    return 0;
}

uint32_t TJSONProtocol::writeListBegin_virt(
        const ::apache::thrift::protocol::TType elemType, const uint32_t size) {

    json_ << " { \"list\" : { \"elemType\" : \"" << elemType << "\", \"size\" : \"" << size << "\", \"value\" : [ ";

    for (int i = 0; size > 0 && i < size - 1; i++) {
        delim_.push(", ");
    }

    end_.push("[");

    return 0;
}

uint32_t TJSONProtocol::writeListEnd_virt() {

    json_ << " ] } } ";
    end_.push("}");

    return 0;
}

uint32_t TJSONProtocol::writeSetBegin_virt(
        const ::apache::thrift::protocol::TType elemType, const uint32_t size) {

    json_ << " { \"set\" : { \"elemType\" : \"" << elemType << "\", \"size\" : \"" << size << "\", \"value\" : [ ";

    for (int i = 0; size > 0 && i < size - 1; i++) {
        delim_.push(", ");
    }

    end_.push("[");

    return 0;
}

uint32_t TJSONProtocol::writeSetEnd_virt() {

    json_ << " ] } } ";
    end_.push("}");

    return 0;
}

uint32_t TJSONProtocol::writeBool_virt(const bool value) {
    json_ << "\"" << value << "\"";
    delim();
    return 0;
}

uint32_t TJSONProtocol::writeByte_virt(const int8_t byte) {
    json_ << "\"" << byte << "\"";
    delim();
    return 0;
}

uint32_t TJSONProtocol::writeI16_virt(const int16_t i16) {
    json_ << "\"" << i16 << "\"";
    delim();
    return 0;
}

uint32_t TJSONProtocol::writeI32_virt(const int32_t i32) {
    json_ << "\"" << i32 << "\"";
    delim();
    return 0;
}

uint32_t TJSONProtocol::writeI64_virt(const int64_t i64) {
    json_ << "\"" << i64 << "\"";
    delim();
    return 0;
}

uint32_t TJSONProtocol::writeDouble_virt(const double dub) {
    json_ << "\"" << dub << "\"";
    delim();
    return 0;
}

uint32_t TJSONProtocol::writeString_virt(const std::string& str) {
    json_ << "\"" << str << "\"";
    delim();
    return 0;
}

uint32_t TJSONProtocol::writeBinary_virt(const std::string& str) {
    json_ << "\"" << str << "\"";
    delim();
    return 0;
}



uint32_t TJSONProtocol::readMessageBegin_virt(std::string& name,
        ::apache::thrift::protocol::TMessageType& messageType, int32_t& seqid) {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readMessageEnd_virt() {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readStructBegin_virt(std::string& name) {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readStructEnd_virt() {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readFieldBegin_virt(std::string& name,
        ::apache::thrift::protocol::TType& fieldType, int16_t& fieldId) {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readFieldEnd_virt() {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readMapBegin_virt(
        ::apache::thrift::protocol::TType& keyType,
         ::apache::thrift::protocol::TType& valType, uint32_t& size) {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readMapEnd_virt() {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readListBegin_virt(
        ::apache::thrift::protocol::TType& elemType, uint32_t& size) {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readListEnd_virt() {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readSetBegin_virt(
        ::apache::thrift::protocol::TType& elemType, uint32_t& size) {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readSetEnd_virt() {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readBool_virt(bool& value) {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readBool_virt(std::vector<bool>::reference value) {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readByte_virt(int8_t& byte) {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readI16_virt(int16_t& i16) {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readI32_virt(int32_t& i32) {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readI64_virt(int64_t& i64) {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readDouble_virt(double& dub) {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readString_virt(std::string& str) {
    DLOGF << std::endl;
    return 0;
}

uint32_t TJSONProtocol::readBinary_virt(std::string& str) {
    DLOGF << std::endl;
    return 0;
}

} /* namespace TRADE */
} /* namespace SF */
