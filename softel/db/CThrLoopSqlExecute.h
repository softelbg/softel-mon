/*
 * CThrLoopSqlExecute.h
 *
 *  Created on: 15.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTHRLOOPSQLEXECUTE_H_
#define CTHRLOOPSQLEXECUTE_H_

#include "CQueueSql.h"
#include "CThreads.h"


namespace SF {
namespace DB {

class CThrLoopSqlExecute : public CThreads
{

public:

	CThrLoopSqlExecute();
	CThrLoopSqlExecute(const int i, CQueueSql* queueSql);
	~CThrLoopSqlExecute();

protected:

	int run();

private:

	int 		m_index;

	CQueueSql*	m_pQueueSql;


};

} /* namespace DB */
} /* namespace SF */

#endif /* CTHRLOOPSQLEXECUTE_H_ */
