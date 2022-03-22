/*
 * CThrLoopSqlExecute.cc
 *
 *  Created on: 15.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CThrLoopSqlExecute.h"

using namespace std;

namespace SF {
namespace DB {

CThrLoopSqlExecute::CThrLoopSqlExecute() {

}

CThrLoopSqlExecute::CThrLoopSqlExecute(const int i, CQueueSql* queueSql) :
	m_index(i) {

	m_pQueueSql = queueSql;

}

CThrLoopSqlExecute::~CThrLoopSqlExecute() {

}

int CThrLoopSqlExecute::run() {

	while(true) {



		if (shouldStop()) {
			break;
		}

		usleep(100000);
	}

	return 0;
}

} /* namespace DB */
} /* namespace SF */
