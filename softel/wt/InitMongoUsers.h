#ifndef INIT_MONGOUSERS_H
#define INIT_MONGOUSERS_H

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

class InitMongoUsers
{
public:
    InitMongoUsers(const std::string& db, const std::string& coll);
    ~InitMongoUsers();
    InitMongoUsers(const InitMongoUsers& r) = delete;
    const InitMongoUsers& operator=(const InitMongoUsers& r) = delete;
    InitMongoUsers(InitMongoUsers&& move) = default;
public:
    mongocxx::cursor getUsers();
private:
    void init();
    float gen_random_float(float min, float max);
private:
    mongocxx::client m_client;
    std::string m_db;
    std::string m_coll;
};
#endif
