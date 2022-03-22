/*
 * DbPool.h
 *
 *	Database Connection Pool.
 *
 *  Created on: 16.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef DbPool_H_
#define DbPool_H_

#include <softel/core/CObject.h>
#include <softel/core/ManagerBase.h>
#include <softel/core/intf/CListInterface.h>
#include <softel/core/commonDefs.h>
#include <softel/db/CDataSet.h>
#include <softel/db/BDbSqlBase.h>
#include <softel/db/CParamSet.h>
#include <softel/core/util/DateTime.h>
#include <softel/core/pool/ThreadPoolAsio.h>

#include <vector>

namespace SF {
namespace CORE {
namespace POOL {


struct DbResult : public SF::CORE::CObject {
	SF::DB::CDataSet ds;
	SF::DB::CParamSet ps;

	std::string toString() const {
		return "DataSet\n" + ds.toString() + "\nParamSet\n" + ps.toString();
	}
};

/**
 * Post CallBack when posting query
 */
class IDbPostCallBackInterface {
public:
	virtual ~IDbPostCallBackInterface() {}

	virtual void onPost() = 0;
	virtual void onError(DbResult& result) = 0;
	virtual void onFinished(DbResult& result) = 0;
};

class DbPoolPostCallBackBase : public virtual IDbPostCallBackInterface {
public:
	virtual ~DbPoolPostCallBackBase() {}

	virtual void onPost();
	virtual void onError(DbResult& result);
	virtual void onFinished(DbResult& result);
};


/*
 * Database Connection Pool.
 * TODO: Should decide what thread pool to be used....
 *
 * TODO: Should make proper DB TABLE representation (DataSet is ok, but something more specific)
 */
class DbPool: 	public SF::CORE::ManagerBase,
				public virtual SF::CORE::INTF::CListInterface {
public:

	enum DbType {
		MySql, Postgresql
	};

	DbPool(enum DbType t, size_t numconn);
	DbPool(enum DbType t, size_t numconn, SF::CORE::Ptr<ThreadPoolAsio>& threadPool);
	virtual ~DbPool();

	void init();
	void init(const std::string& host, const std::string& dbname,
              const std::string& user, const std::string& pass);

	void post(const std::string& query, SF::CORE::Ptr<IDbPostCallBackInterface>& cb);
	bool execute(const std::string& query, DbResult& result);

	bool doSelect(const std::string& query, DbResult& result);

	bool isInitialized() {
		return m_flag_initialized;
	}

	void refresh();

	size_t size() const;
	void clear();

	virtual std::string toString() const;
	virtual std::string toXml() const;

private:
	size_t getConnIdx();
	SF::CORE::Ptr<SF::DB::BDbSqlBase> getConn();
	bool getConn(SF::CORE::Ptr<SF::DB::BDbSqlBase>& c);

	enum DbType m_type;
	size_t m_num_connections;

	std::vector< SF::CORE::Ptr<SF::DB::BDbSqlBase> > m_conn_pool;
	size_t m_next_conn;

	SF::CORE::Ptr<ThreadPoolAsio> m_thread_pool;

	bool m_flag_initialized;

	SF::CORE::UTIL::DateTime m_date_init;

	boost::atomic<int> m_used_conn;

};

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */
#endif /* DbPool_H_ */
