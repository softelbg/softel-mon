/*
 * WorkItemSqlPost.h
 *
 *  Created on: 27.08.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 - 2013 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef WorkItemSqlPost_H_
#define WorkItemSqlPost_H_

#include <softel/db/WorkItemQueryBase.h>
#include <softel/db/BDbSqlBase.h>
#include <softel/core/pool/DbPool.h>
#include <softel/core/error/CErrorSql.h>

namespace SF {
namespace CORE {
namespace POOL {

/*
 *
 */
class WorkItemSqlPost: public SF::DB::WorkItemQueryBase {
public:
	WorkItemSqlPost(const std::string& query,
			SF::CORE::Ptr<IDbPostCallBackInterface>& cb, SF::CORE::Ptr<SF::DB::BDbSqlBase>& c);
	virtual ~WorkItemSqlPost();

	virtual void run();

private:
	SF::CORE::Ptr<SF::DB::BDbSqlBase> m_db_conn;
	SF::CORE::Ptr<IDbPostCallBackInterface> m_callback;
};

} /* namespace POOL */
} /* namespace CORE */
} /* namespace SF */

#endif /* WorkItemSqlPost_H_ */
