/*
 * CmtsList.h
 * List of CMTS
 * Retrieved from MySql DB
 *
 *  Created on: 13.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCmtsList_H
#define CCmtsList_H

#include <stdio.h>
#include <sys/wait.h>

#include <map>
#include <vector>
#include <string>

#include "CCmts.h"
#include "CCmtsToolsSql.h"
#include "CCmtsFileIni.h"
#include "CCmtsLog.h"

#include <softel/core/inc/ErrorMsg.h>
#include <softel/db/CSqlDbFactory.h>
#include <softel/db/CMySqlDB.h>
#include <softel/core/net/CIp.h>
#include <softel/core/error/CErrorCommon.h>
#include <softel/core/util/CSharedCounter.h>
#include <softel/db/CDataSet.h>


namespace SF {
namespace OSS {
namespace CMTS {

class CCmtsList
{
public:
	CCmtsList();
		
	~CCmtsList();

	void initCmtsList();

	void listCmts();
	void listCmts(SF::CORE::UTIL::CSharedCounter& processCount);

	void launchCmts();

	void addCmts(CCmts*);

	void dumpCmtsList();

private:

	void getCmtsList(std::vector<CCmts*>& cmtsList);
	void getCmtsList(SF::DB::CMySqlDB* pDbConn, std::vector<CCmts*>& cmtsList);

	int				m_periodSleep;
	int				m_pidMainProcess;

	SF::CORE::UTIL::CSharedCounter	m_processCounter;

	SF::DB::CSqlDbFactory* 	m_pDbFactory;
	SF::DB::CSqlConnPool*	m_pDbConnPool;
	SF::DB::CMySqlDB* 		m_pDbConn;
	
	std::vector<CCmts*>	m_cmts_list;

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif
