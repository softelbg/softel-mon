/*
 * TableBase.h
 *
 *	DB Table representation. Should looks like a sql db table with rows/columns.
 *	Currently it is a CDataSet...
 *
 *  Created on: May 3, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef TABLEBASE_H_
#define TABLEBASE_H_

#include "CDataSet.h"

namespace SF {
namespace DB {

class TableBase: public CDataSet {
public:
	TableBase(const string& nameTable);
	virtual ~TableBase();

	std::string getTableName() const {
		return m_table_name;
	}

private:
	std::string m_table_name;
};

} /* namespace DB */
} /* namespace SF */
#endif /* TABLEBASE_H_ */
