/*
 * CDataRow.h
 *
 *	Represents Row of a set of data - contains row with column->values list
 *
 *  Created on: 28.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CDATAROW_H_
#define CDATAROW_H_

#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>


namespace SF {
namespace DB {

class CDataRow {

public:
	CDataRow();
	CDataRow(const std::vector<std::string>& listKeys, const std::vector<std::string>& listValues);
	~CDataRow();

	bool init(const std::vector<std::string>& listKeys, const std::vector<std::string>& listValues);

	void addColumn(const std::string& colname, const std::string& val);

	std::string getColumn(const std::string& colname) const;
	bool getColumn(const std::string& colname, std::string& result) const;
	bool getColumn(const int colpos, std::string& colname, std::string& result) const;
	bool getNextColumn(std::string& colname, std::string& result);

	int getNumColumn() const;

	void getColumnNamesCSV(std::string& result) const;
	void getColumnValuesCSV(std::string& result, const std::string& delimVal) const;
	void getColumnsCSV(std::string& result, const std::string& delimVal) const;

	void incPosColumn();
	void resetColPos();

	void appendRow(CDataRow& dstRow) const;
	void copyRow(CDataRow& dstRow) const;

	void clear();
	int size() const;

	void dumpInfo() const;

private:

	int m_current_column;

	std::vector<std::string> m_data_row;
	// Indexes
	std::map<std::string, int> m_idx_colname;
	std::map<int, std::string> m_idx_colpos;


};

} /* namespace DB */
} /* namespace SF */

#endif /* CDATAROW_H_ */
