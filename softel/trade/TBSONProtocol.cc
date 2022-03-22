/*
 * TBSONProtocol.cc
 *
 *  Created on: May 13, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include <softel/trade/TBSONProtocol.h>

using namespace std;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;


namespace SF {
namespace TRADE {

TBSONProtocol::TBSONProtocol()
: TProtocol(NULL){
    DLOGF << std::endl;
    pushDocument("root");
}

TBSONProtocol::~TBSONProtocol() {
    DLOGF << std::endl;
}

void TBSONProtocol::pushArray(const std::string& name) {
    DLOGF << std::endl;
    assert(!obj_.empty());
    obj_.push(BSONObj(name, BSONObj::Types::ARRAY));
}

void TBSONProtocol::pushDocument(const std::string& name) {
    DLOGF << std::endl;
    obj_.push(BSONObj(name, BSONObj::Types::DOCUMENT));
}

void TBSONProtocol::popDocument() {
    DLOGF << std::endl;
    assert(!obj_.empty());
    /*
    BSONObj obj = obj_.top();
    assert(obj.type == BSONObj::Types::DOCUMENT);
    obj_.pop();
    assert(!obj_.empty());
    BSONObj& objParent = obj_.top();

    switch(objParent.type) {
        case BSONObj::Types::ARRAY :
        {
            objParent.arr.append(bsoncxx::builder::basic::kvp(obj.name,
                    [&](bsoncxx::builder::basic::sub_document subdoc) {
                subdoc.append(obj.doc);

            })
            );
        }

        case BSONObj::Types::DOCUMENT :
        {

        }
    }
*/

}

void TBSONProtocol::popArray() {
    DLOGF << std::endl;
    /*
    assert(!arrays_.empty());
    arrays_.pop();
    assert(!obj_.empty());
    obj_.top() << bsoncxx::builder::stream::close_array;
     */
}

uint32_t TBSONProtocol::writeMessageBegin_virt(const std::string& name,
        const ::apache::thrift::protocol::TMessageType messageType,
        const int32_t seqid) {
    DLOGF << std::endl;
    return 0;
}

uint32_t TBSONProtocol::writeMessageEnd_virt() {
    DLOGF << std::endl;
    return 0;
}

uint32_t TBSONProtocol::writeStructBegin_virt(const char* name) {
    assert(!obj_.empty());
    pushDocument(name);
    return 0;
}

uint32_t TBSONProtocol::writeStructEnd_virt() {
    assert(!obj_.empty());
    popDocument();
    return 0;
}

uint32_t TBSONProtocol::writeFieldBegin_virt(const char* name,
        const ::apache::thrift::protocol::TType fieldType,
        const int16_t fieldId) {
    assert(!obj_.empty());

    obj_.top().append("id", fieldId);
    obj_.top().append("type", fieldType);

    pushDocument(name);

    return 0;
}

uint32_t TBSONProtocol::writeFieldEnd_virt() {
    assert(!obj_.empty());
    popDocument();
    return 0;
}

uint32_t TBSONProtocol::writeFieldStop_virt() {
    DLOGF << std::endl;
    return 0;
}

uint32_t TBSONProtocol::writeMapBegin_virt(
        const ::apache::thrift::protocol::TType keyType,
        const ::apache::thrift::protocol::TType valType, const uint32_t size) {
    assert(!obj_.empty());
    obj_.top().append("keyType", keyType);
    obj_.top().append("valType", valType);
    obj_.top().append("size", std::to_string(size));
    pushDocument("map");
    return 0;
}

uint32_t TBSONProtocol::writeMapEnd_virt() {
    popDocument();
    return 0;
}

uint32_t TBSONProtocol::writeListBegin_virt(
        const ::apache::thrift::protocol::TType elemType, const uint32_t size) {
    assert(!obj_.empty());
    obj_.top().append("elemType", elemType);
    obj_.top().append("size", std::to_string(size));
    pushArray("list");
    return 0;
}

uint32_t TBSONProtocol::writeListEnd_virt() {
    popArray();
    return 0;
}

uint32_t TBSONProtocol::writeSetBegin_virt(
        const ::apache::thrift::protocol::TType elemType, const uint32_t size) {
    assert(!obj_.empty());
    obj_.top().append("elemType", elemType);
    obj_.top().append("size", std::to_string(size));
    pushArray("set");
    return 0;
}

uint32_t TBSONProtocol::writeSetEnd_virt() {
    popArray();
    return 0;
}

uint32_t TBSONProtocol::writeBool_virt(const bool value) {
    assert(!obj_.empty());
    obj_.top().append(std::to_string(value));
    return 0;
}

uint32_t TBSONProtocol::writeByte_virt(const int8_t byte) {
    assert(!obj_.empty());
    obj_.top().append(byte);
    return 0;
}

uint32_t TBSONProtocol::writeI16_virt(const int16_t i16) {
    assert(!obj_.empty());
    obj_.top().append(i16);
    return 0;
}

uint32_t TBSONProtocol::writeI32_virt(const int32_t i32) {
    assert(!obj_.empty());
    obj_.top().append(i32);
    return 0;
}

uint32_t TBSONProtocol::writeI64_virt(const int64_t i64) {
    assert(!obj_.empty());
    obj_.top().append(i64);
    return 0;
}

uint32_t TBSONProtocol::writeDouble_virt(const double dub) {
    assert(!obj_.empty());
    obj_.top().append(dub);
    return 0;
}

uint32_t TBSONProtocol::writeString_virt(const std::string& str) {
    assert(!obj_.empty());
    obj_.top().append(str);
    return 0;
}

uint32_t TBSONProtocol::writeBinary_virt(const std::string& str) {
    assert(!obj_.empty());
    obj_.top().append(str);
    return 0;
}


uint32_t TBSONProtocol::readMessageBegin_virt(std::string& name,
        ::apache::thrift::protocol::TMessageType& messageType, int32_t& seqid) {

    return 0;
}

uint32_t TBSONProtocol::readMessageEnd_virt() {

    return 0;
}

uint32_t TBSONProtocol::readStructBegin_virt(std::string& name) {

    return 0;
}

uint32_t TBSONProtocol::readStructEnd_virt() {

    return 0;
}

uint32_t TBSONProtocol::readFieldBegin_virt(std::string& name,
        ::apache::thrift::protocol::TType& fieldType, int16_t& fieldId) {

    return 0;
}

uint32_t TBSONProtocol::readFieldEnd_virt() {

    return 0;
}

uint32_t TBSONProtocol::readMapBegin_virt(
        ::apache::thrift::protocol::TType& keyType,
        ::apache::thrift::protocol::TType& valType, uint32_t& size) {

    return 0;
}

uint32_t TBSONProtocol::readMapEnd_virt() {

    return 0;
}

uint32_t TBSONProtocol::readListBegin_virt(
        ::apache::thrift::protocol::TType& elemType, uint32_t& size) {

    return 0;
}

uint32_t TBSONProtocol::readListEnd_virt() {

    return 0;
}

uint32_t TBSONProtocol::readSetBegin_virt(
        ::apache::thrift::protocol::TType& elemType, uint32_t& size) {

    return 0;
}

uint32_t TBSONProtocol::readSetEnd_virt() {

    return 0;
}

uint32_t TBSONProtocol::readBool_virt(bool& value) {

    return 0;
}

uint32_t TBSONProtocol::readBool_virt(std::vector<bool>::reference value) {

    return 0;
}

uint32_t TBSONProtocol::readByte_virt(int8_t& byte) {

    return 0;
}

uint32_t TBSONProtocol::readI16_virt(int16_t& i16) {

    return 0;
}

uint32_t TBSONProtocol::readI32_virt(int32_t& i32) {

    return 0;
}

uint32_t TBSONProtocol::readI64_virt(int64_t& i64) {

    return 0;
}

uint32_t TBSONProtocol::readDouble_virt(double& dub) {

    return 0;
}

uint32_t TBSONProtocol::readString_virt(std::string& str) {

    return 0;
}

uint32_t TBSONProtocol::readBinary_virt(std::string& str) {

    return 0;
}


} /* namespace TRADE */
} /* namespace SF */
