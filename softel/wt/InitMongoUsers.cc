#include "InitMongoUsers.h"

#include <random>
#include <iostream>
#include "common.h"

InitMongoUsers::InitMongoUsers(const std::string& db, const std::string& coll)
    : m_db(db),
      m_coll(coll)
{
    mongocxx::instance inst{};
    m_client = std::move(mongocxx::uri{});
    init();
}

InitMongoUsers::~InitMongoUsers()
{
    
}

void InitMongoUsers::init()
{
    
    std::vector<std::string> machines;
    machines.push_back(FOREX_WIDGET);
    machines.push_back(TRADING_WIDGET);
    machines.push_back(ITEM_TRADE_WIDGET);
   
    auto random = [](int min, int max)
        {
            return min + (rand() % (int)(max - min + 1));
        };
    // clear
    bsoncxx::builder::stream::document filter_builder{};
    auto db = m_client[m_db][m_coll];
    db.delete_many(std::move(filter_builder.view()));
    
    for (std::size_t idx = 1; idx <= 15; ++idx)
    {
        
        
        std::string user = "User" + std::to_string(idx);
        
        bsoncxx::builder::stream::document doc;
        doc << "user" << user << "balance" << gen_random_float(2131243.237,6131243.637 )
            << "trademachine" << machines[random(0,2)];
      
        db.insert_one(std::move(doc.view()));
    }
}

mongocxx::cursor InitMongoUsers::getUsers()
{
    bsoncxx::builder::stream::document doc{};
    auto db = m_client[m_db][m_coll];
    return std::move(db.find(doc.view()));
}

float InitMongoUsers::gen_random_float(float min, float max)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> u(min, max);
    return u(rng);
   
}
