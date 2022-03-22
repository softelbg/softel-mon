/*
 * CSqlToolsBase.h
 *
 *	Base SQL Tools
 *
 *  Created on: 01.08.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSQLTOOLSBASE_H_
#define CSQLTOOLSBASE_H_

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

#include <softel/db/CMySqlDB.h>
#include "CMySqlConn.h"

namespace SF {
namespace OSS {
namespace CMTS {

class CSqlToolsBase {

public:
	CSqlToolsBase();
	CSqlToolsBase(int countConn);
	~CSqlToolsBase();

	void init(int countConn);

	bool sqlExecute(SF::DB::CMySqlDB* pDb, const std::string& query);
	bool sqlExecute(SF::DB::CMySqlDB* pDb, const std::stringstream& query);
	bool sqlExecute(const std::string& query);
	bool sqlExecute(const std::stringstream& query);
	long sqlInsert(SF::DB::CMySqlDB* pDb, const std::stringstream& query);
	long sqlInsert(const std::stringstream& query);
	long sqlInsert(const std::string& query);
	long sqlUpdate(const std::stringstream& query);
	long sqlUpdate(const std::string& query);
	bool sqlSelect(SF::DB::CMySqlDB* pDb, const std::stringstream& query, SF::DB::CDataSet& ds);
	bool sqlSelect(const std::stringstream& query, SF::DB::CDataSet& ds);

private:

	SF::DB::CMySqlDB*	m_pDb;

};


} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CSQLTOOLSBASE_H_ */
