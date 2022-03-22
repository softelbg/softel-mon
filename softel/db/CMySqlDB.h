#ifndef CMySqlDB_H
#define CMySqlDB_H

#include "CDbMySql.h"

namespace SF {
namespace DB {

class CMySqlDB : public CDbMySql {
public:
	CMySqlDB(const std::string& Host, const std::string& User, const std::string& Password,
				const std::string& dbname);

	~CMySqlDB();


};

} /* namespace DB */
} /* namespace SF */

#endif

