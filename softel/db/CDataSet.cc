/*
 * CDataSet.cc
 *
 *	Data Set container - should represent for example DB data set with columns and rows.
 *	Using only string type (all other types should be converted into it)
 *
 *  Created on: 21.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CDataSet.h"

#include <softel/core/util/CStringUtil.h>
#include <sstream>
#include <sys/types.h>
#include <unistd.h>
	   
using namespace std;
using namespace SF::CORE::UTIL;

namespace SF {
namespace DB {

CDataSet::CDataSet() {

	clear();

}

CDataSet::~CDataSet() {

	clear();

}


void CDataSet::clear() {

	m_data_set.clear();

	m_custom_column_idx.clear();
	m_idx_column.clear();
	m_idx_colname.clear();

	m_num_columns = 0;
	m_num_rows = 0;
	m_current_row = 0;

	m_flagCreateIndex = false;
}

/**
 * Set Create Index Flag
 */
void CDataSet::setCreateIndex(bool val = true) {

	m_flagCreateIndex = val;

}


/**
 * Build new column Index (if already existed it is rebuild)
 */
void CDataSet::buildColumnIndex(const string& col) {

	vector<string>* pCol = NULL;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":pid:" << getpid() << endl;

	map<string, int>& rIdx = m_custom_column_idx[col];

	rIdx.clear();

	if (getColumn(col, &pCol) == true) {

		vector<string>& rCol = *pCol;

		for(unsigned int i = 0; i < rCol.size(); i++) {
			rIdx[rCol[i]] = i;
		}

	} else {
		cout << __FUNCTION__ << ": No Column:" << col << endl;
	}

}

/**
 * Search an indexed column for a key.
 * Return position or -1 if none
 */
int CDataSet::searchIndexedColumn(const string& col, const string& key) const {

	int pos = -1;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":pid:" << getpid();
	//cout << ": col:" << col << " key:" << key << endl;

	map< string, map<string, int> >::const_iterator it_col;
	map<string, int>::const_iterator it_key;

	it_col = m_custom_column_idx.find(col);

	if (it_col != m_custom_column_idx.end()) {

		it_key = it_col->second.find(key);

		if (it_key != it_col->second.end()) {

			pos = it_key->second;

		} else {
			pos = -1;
		}

	} else {
		pos = -1;
	}

	if (pos < 0) {
		//cout << __FUNCTION__ << ": Not Found Column:" << col << " or Key:" << key << endl;
	}

	return pos;
}


/**
 * Add new Empty Column to the Data Set
 */
void CDataSet::addColumn(const string& col) {

	vector<string> emptyCol;

	this->addColumn(col, emptyCol);
}

/**
 * Add new Empty Columns to the Data Set from a Column Names list.
 */
void CDataSet::addColumn(const vector<string>& listCol) {

	vector<string> emptyCol;

	for(int i = 0; i < listCol.size(); i++) {
		this->addColumn(listCol[i], emptyCol);
	}

}

/**
 * Add new Column to the Data Set (filled from a vector)
 */
void CDataSet::addColumn(const string& col, const vector<string>& colValues) {

	unsigned int pos = m_data_set.size();

	m_data_set.push_back(colValues);

	m_idx_column[col] = pos;
	m_idx_colname[pos] = col;

	m_num_columns = m_data_set.size();
}

/**
 * Add a Row to the Data Set from vector (columns are ordered...)
 */
void CDataSet::addRow(const vector<string>& rowValues) {

	unsigned int i;

	for(i = 0; i < rowValues.size(); i++) {
		m_data_set[i].push_back(rowValues[i]);
	}

	m_num_rows++;

}

int CDataSet::size() const {
	return getRowNum();
}

int CDataSet::getRowNum() const {

	int numRows = 0;

	if (m_data_set.size() > 0) {
		numRows = m_data_set[0].size();
	}

	//m_num_rows = numRows;

	return numRows;
}

void CDataSet::incRowPos() {
	m_current_row++;

	if (m_current_row >= getRowNum()) {
		//resetRowPos();
	}
}

void CDataSet::resetRowPos() {
	m_current_row = 0;
}

/**
 * Get Row from the Data Set as a Map(containing colname = colvalue)
 */
bool CDataSet::getRow(const int posRow, map<string, string>& resultRow) const {

	unsigned int i;

	resultRow.clear();

	if (posRow >= getRowNum()) {
		return false;
	}

	map<int, string>::const_iterator it;

	for(i = 0; i < m_data_set.size(); i++) {

		// Skip non-existent rows
		if (posRow >= m_data_set[i].size()) {
			continue;
		}

		it = m_idx_colname.find(i);

		if (it != m_idx_colname.end()) {
			resultRow[it->second] = m_data_set[i][posRow];
		}

	}

	return true;
}

bool CDataSet::getRow(const int posRow, CDataRow& resultRow) const {

	unsigned int i;

	resultRow.clear();

	if (posRow < 0 || posRow >= getRowNum()) {
		return false;
	}

	map<int, string>::const_iterator it;

	for (i = 0; i < m_data_set.size(); i++) {

		// Skip non-existent rows
		if (posRow >= m_data_set[i].size()) {
			continue;
		}

		it = m_idx_colname.find(i);

		if (it != m_idx_colname.end()) {
			resultRow.addColumn(it->second, m_data_set[i][posRow]);
		}

	}

	return true;
}

bool CDataSet::getNextRow(CDataRow& resultRow) {

	bool flagResult = getRow(m_current_row, resultRow);

	incRowPos();

	return flagResult;
}

/**
 * Return column vector.
 * TODO: Consider use map find...
 */
bool CDataSet::getColumn(const string& col, vector<string>** pResultColumn) {

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":pid:" << getpid() << ": get col:" << col << endl;

	map<string, int>::const_iterator it;

	it = m_idx_column.find(col);

	if (it != m_idx_column.end()) {
		*pResultColumn = &(m_data_set[it->second]);
		return true;
	} else {
		pResultColumn = NULL;
		return false;
	}

}

void CDataSet::buildColumnIndex(const string& col1, const string& col2) {

	vector<string> listCol;

	listCol.push_back(col1);
	listCol.push_back(col2);

	this->buildColumnIndex(listCol);
}

void CDataSet::buildColumnIndex(const vector<string>& listCol) {

	string keyCol;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":pid:" << getpid() << endl;

	this->doKey(listCol, keyCol);

	map<string, int>& rIdx = m_custom_column_idx[keyCol];

	rIdx.clear();

	CDataRow row;

	for(int i = 0; i < this->size(); i++) {

		this->getRow(i, row);

		vector<string> listKeys;

		for(size_t j = 0; j < listCol.size(); j++) {

			string key;

			row.getColumn(listCol[j], key);

			listKeys.push_back(key);
		}

		string keyKey;

		this->doKey(listKeys, keyKey);

		rIdx[keyKey] = i;

	}

}

int CDataSet::searchIndexedColumn(const vector<string>& listCol, const vector<string>& listKey) const {

	int pos = -1;

	//cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":pid:" << getpid();
	//cout << ": col:" << col << " key:" << key << endl;

	string keyCol;
	string keyKey;

	this->doKey(listCol, keyCol);
	this->doKey(listKey, keyKey);

	return this->searchIndexedColumn(keyCol, keyKey);
}

string CDataSet::toString() const {

	stringstream ss;

	int numRows = getRowNum();
	int i;
	map<string, string> mapRow;
	map<string, string>::iterator it;

	ss << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid() << " -------------------------- START --------" << endl;

	for(i = 0; i < numRows; i++) {

		ss << "ROW:" << i << " ";

		getRow(i, mapRow);

		for(it = mapRow.begin(); it != mapRow.end(); it++) {

			ss << it->first << "=" << it->second << " ";
		}

		ss << endl;
	}

	ss << "--------- INDEXES -----------" << endl;

	map< string, map<string, int> >::const_iterator idx_it;

	for(idx_it = m_custom_column_idx.begin(); idx_it != m_custom_column_idx.end(); idx_it++) {

		ss << "\tINDEX on " << idx_it->first << endl;

		map<string, int>::const_iterator it_col;
		for(it_col = idx_it->second.begin(); it_col != idx_it->second.end(); it_col++) {

			map<string, int>::const_iterator it_idx_col = m_idx_column.find(idx_it->first);

			if (it_idx_col != m_idx_column.end()) {
				ss << "\t\t" << it_col->first << " = " << it_col->second;
				ss << " [" << m_data_set[it_idx_col->second][it_col->second] << "]" << endl;
			}

		}

	}

	ss << "--------- END INDEXES -----------" << endl;

	ss << __FILE__ << ":" << __FUNCTION__ << ":pid:" << getpid() << " -------------------------- END ----------" << endl;

	return ss.str();
}

void CDataSet::dumpInfo() const {
	cout << toString();
}

bool CDataSet::getRow(const vector<string>& listCol,
		const vector<string>& listKey, CDataRow& resultRow) const {

	int pos = this->searchIndexedColumn(listCol, listKey);

	return this->getRow(pos, resultRow);
}

int CDataSet::searchIndexedColumn(const string& col1, const string& col2,
		const string& key1, const string& key2) {

	vector<string> listCol;
	vector<string> listKey;

	listCol.push_back(col1);
	listCol.push_back(col2);

	listKey.push_back(key1);
	listKey.push_back(key2);

	return this->searchIndexedColumn(listCol, listKey);
}

/**
 * Add a new Row
 */
void CDataSet::addRow(const CDataRow& row) {

	for(int i = 0; i < row.size(); i++) {

		string col;
		string val;

		row.getColumn(i, col, val);

		this->addColumnValue(col, val);
	}
}

/**
 * Search for the Column index and return if found.
 */
int CDataSet::getColumnIndex(const string& col) const {

	map<string, int>::const_iterator it = this->m_idx_column.find(col);

	if (it != this->m_idx_column.end()) {
		return it->second;
	} else {
		return -1;
	}
}

/**
 * Add a value to the column.
 */
bool CDataSet::addColumnValue(const string& col, const string& value) {

	int pos = this->getColumnIndex(col);

	if (pos >= 0) {

		this->m_data_set[pos].push_back(value);

	} else {
		return false;
	}

	return true;
}

/**
 * Do a single Key from a List of Keys.
 * Just concatenate all keys and use some clue char.
 */
void CDataSet::doKey(const vector<string>& listKeys, string& resultKey) const {

	resultKey.clear();

	CStringUtil::implode(listKeys, "_", resultKey);

}

} /* namespace DB */
} /* namespace SF */


