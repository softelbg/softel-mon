/*
 * CDataRow.cc
 *
 *  Created on: 28.02.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CDataRow.h"

using namespace std;

namespace SF {
namespace DB {

CDataRow::CDataRow() {

	clear();

}

CDataRow::~CDataRow() {

	clear();

}

/**
 * Add new Column as name -> value
 */
void CDataRow::addColumn(const string& colname, const string& val) {

	unsigned int pos;

	pos = m_data_row.size();

	map<string, int>::iterator it;

	it = m_idx_colname.find(colname);

	// If the column does not exists then add it
	if (it == m_idx_colname.end()) {

		m_data_row.push_back(val);
		m_idx_colname[colname] = pos;
		m_idx_colpos[pos] = colname;

	} else {
		m_data_row[it->second] = val;
	}

}

/**
 * Get Column value by column name.
 * Return string via Stack...
 */
string CDataRow::getColumn(const string& colname) const {

	string result;

	if (getColumn(colname, result) == false) {
		result = "";
	}

	return result;
}
/**
 * Get Column value by column name
 */
bool CDataRow::getColumn(const string& colname, string& result) const {

	map<string, int>::const_iterator it;

	it = m_idx_colname.find(colname);

	if (it != m_idx_colname.end()) {
		result = m_data_row[it->second];
		return true;
	} else {
		return false;
	}

}

/**
 * Get Column properties (name and value) by column position.
 */
bool CDataRow::getColumn(const int colpos, string& colname, string& result) const {

	map<int, string>::const_iterator it;

	it = m_idx_colpos.find(colpos);

	if (it != m_idx_colpos.end()) {
		result = m_data_row[colpos];
		colname = it->second;
		return true;
	} else {
		return false;
	}

}

/**
 * Get Next Column
 */
bool CDataRow::getNextColumn(string& colname, string& result) {

	bool resultFlag = getColumn(m_current_column, colname, result);

	incPosColumn();

	return resultFlag;
}

int CDataRow::size() const {
	return m_data_row.size();
}

int CDataRow::getNumColumn() const {
	return this->size();
}

void CDataRow::incPosColumn() {

	m_current_column++;

	if (m_current_column >= getNumColumn()) {
		//resetColPos();
	}

}

/**
 * Get Column Names as ", " separated values
 */
void CDataRow::getColumnNamesCSV(string& result) const {

	unsigned int i;

	map<int, string>::const_iterator it;

	for(i = 0; i < getNumColumn(); i++) {

		it = m_idx_colpos.find(i);

		if (it != m_idx_colpos.end()) {
			result += it->second;
		}

		if (i < getNumColumn() - 1) {
			result += ", ";
		}

	}

}

/**
 * Get Column Values as ", " separated values
 */
void CDataRow::getColumnValuesCSV(string& result, const string& delimVal) const {

	unsigned int i;

	for (i = 0; i < getNumColumn(); i++) {

		result += delimVal + m_data_row[i] + delimVal;

		if (i < getNumColumn() - 1) {
			result += ", ";
		}

	}

}

/**
 * Get Column Names = Column Values as ", " separated values
 */
void CDataRow::getColumnsCSV(string& result, const string& delimVal) const {

	unsigned int i;

	map<int, string>::const_iterator it;

	for (i = 0; i < getNumColumn(); i++) {

		it = m_idx_colpos.find(i);

		if (it != m_idx_colpos.end()) {
			result += it->second;
			result += " = ";
			result += delimVal + m_data_row[i] + delimVal;
		}

		if (i < getNumColumn() - 1) {
			result += ", ";
		}

	}

}

/**
 * Append this row to destination (dstRow is NOT Cleared, so it could has other stuff unchanged/changed)
 */
void CDataRow::appendRow(CDataRow& dstRow) const {

	string colName;
	string colResult;

	for(int i = 0; i < this->size(); i++) {
		this->getColumn(i, colName, colResult);
		dstRow.addColumn(colName, colResult);
	}

}

/**
 * Copy This Row to dstRow (dstRow is Cleared)
 */
void CDataRow::copyRow(CDataRow& dstRow) const {

	dstRow.clear();

	this->appendRow(dstRow);

}

void CDataRow::resetColPos() {
	m_current_column = 0;
}

void CDataRow::clear() {

	m_data_row.clear();
	m_idx_colname.clear();
	m_idx_colpos.clear();

	resetColPos();
}

CDataRow::CDataRow(const vector<string>& listKeys,
		const vector<string>& listValues) {

	this->init(listKeys, listValues);
}

/**
 * Initialize the DataRow using complementary lists of key->value
 */
bool CDataRow::init(const vector<string>& listKeys,
		const vector<string>& listValues) {

	if (listKeys.size() != listValues.size()) {
		return false;
	}

	for(int i = 0; i < listKeys.size(); i++) {
		this->addColumn(listKeys[i], listValues[i]);
	}

	return true;
}

void CDataRow::dumpInfo() const {

	unsigned int i;

	cout << __FILE__ << ":" << __FUNCTION__ << " ------------------" << endl;

	map<int, string>::const_iterator it;

	for(i = 0; i < getNumColumn(); i++) {

		it = m_idx_colpos.find(i);

		if (it != m_idx_colpos.end()) {
			cout << it->second << " => " << m_data_row[i] << endl;
		}

	}

}

} /* namespace DB */
} /* namespace SF */

