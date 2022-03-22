#include "UinfoStorage.h"

UinfoStorage::UinfoStorage(const std::string& db , const std::string& coll)
    : m_strDb(db),
      m_strColl(coll)
{
    mongocxx::instance inst{};
    m_client = std::move(mongocxx::uri{});
}

void UinfoStorage::insert(const bsoncxx::builder::stream::document& input)
{
    auto db = m_client[m_strDb][m_strColl];
    db.insert_one(std::move(input.view()));
    Notify<DBObservers::InsertEvent>(input);
}

mongocxx::cursor UinfoStorage::fetch(const bsoncxx::builder::stream::document& query)
{
    auto db = m_client[m_strDb][m_strColl];
    return std::move(db.find(query.view()));
}
