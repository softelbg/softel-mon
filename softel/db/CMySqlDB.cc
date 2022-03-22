
#include "CMySqlDB.h"

namespace SF {
namespace DB {

using namespace std;

CMySqlDB::CMySqlDB(const string& Host, const string& User,
		const string& Password, const string& dbname) :
		CDbMySql(Host, dbname, User, Password) {

}

CMySqlDB::~CMySqlDB() {

}

} /* namespace DB */
} /* namespace SF */
