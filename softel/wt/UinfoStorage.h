#ifndef U_INFOSTORAGE_H
#define U_INFOSTORAGE_H

#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include <softel/core/Observer.h>

struct DBObservers
{
    enum {InsertEvent};
    using ObserverTable = std::tuple<
        boost::signals2::signal<void (const bsoncxx::builder::stream::document& out)>
        >;
};
    
class UinfoStorage : public Observable<DBObservers>
{
public:
    UinfoStorage(const std::string& db , const std::string& coll);
public:
    UinfoStorage(const UinfoStorage& robj) = delete;
    const UinfoStorage& operator=(const UinfoStorage& robj) = delete;
public:
    void insert(const bsoncxx::builder::stream::document& input);
    mongocxx::cursor fetch(const bsoncxx::builder::stream::document& query);
private:
    mongocxx::client m_client;
    std::string m_strDb;
    std::string m_strColl;
};
#endif
