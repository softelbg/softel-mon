/*
 * CDataSet.h
 *
 *  Created on: 21.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CDATASET_H_
#define CDATASET_H_

#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <softel/core/CObject.h>
#include "CDataRow.h"

namespace SF {
namespace DB {

class CDataSet : public SF::CORE::CObject {

public:

	CDataSet();
	~CDataSet();

	void addColumn(const std::string& col);
	void addColumn(const std::vector<std::string>& listCol);
	void addColumn(const std::string& col, const std::vector<std::string>&);

	void addRow(const std::vector<std::string>&);
	void addRow(const CDataRow& row);

	int size() const;
	int getRowNum() const;

	bool getRow(const int posRow, std::map<std::string, std::string>& resultRow) const;
	bool getRow(const int posRow, CDataRow& resultRow) const;
	bool getRow(const std::vector<std::string>& listCol, const std::vector<std::string>& listKey, CDataRow& resultRow) const;

	bool getNextRow(CDataRow& resultRow);

	bool getColumn(const std::string& col, std::vector<std::string>**);
	int getColumnIndex(const std::string& col) const;

	void incRowPos();
	void resetRowPos();

	void setCreateIndex(bool);
	void buildColumnIndex(const std::string& col);
	void buildColumnIndex(const std::string& col1, const std::string& col2);
	void buildColumnIndex(const std::vector<std::string>& listCol);

	int searchIndexedColumn(const std::string& col, const std::string& key) const;
	int searchIndexedColumn(const std::string& col1, const std::string& col2, const std::string& key1, const std::string& key2);
	int searchIndexedColumn(const std::vector<std::string>& listCol, const std::vector<std::string>& listKey) const;

	void clear();

	void dumpInfo() const;
	std::string toString() const;

private:

	bool addColumnValue(const std::string& col, const std::string& value);

	void doKey(const std::vector<std::string>& listKeys, std::string& resultKey) const;

	int m_num_columns;
	int m_num_rows;
	int m_current_row;

	bool m_flagCreateIndex;

	std::vector<std::vector<std::string> > m_data_set;
	std::map<std::string, int>::iterator m_it;
	std::map<std::string, int> m_idx_column;
	std::map<int, std::string> m_idx_colname;

	std::map< std::string, std::map<std::string, int> > m_custom_column_idx;
	std::map< std::string, std::map<std::string, int> >::iterator m_idx_it;

};

} /* namespace DB */
} /* namespace SF */

#endif /* CDATASET_H_ */
