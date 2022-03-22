/*
 * BFileIniBase.cc
 *
 *  Created on: 29.03.2013
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BFileIniBase.h"

using namespace std;

namespace SF {
namespace CORE {
namespace UTIL {

#define KEY_COMMENT 				"#"
#define KEY_PARAM_KEYVAL_SEPARATOR	"="
#define LIST_REMOVE_SYM				" \r\n\t"

BFileIniBase::BFileIniBase(const string& fileName) :
	BFileReaderBase(fileName) {

	this->read();

}

BFileIniBase::~BFileIniBase() {
	// TODO Auto-generated destructor stub
}

/**
 * Fill data row param set with key=value list from the file lines.
 * File Line Processed here one by one.
 * TODO: Consider check/implement adding sections to param list...
 */
void BFileIniBase::processLine(const string& line) {

	//cout << "Line:" << line << endl;

	std::vector<string> listLineTokens;

	// Tokenize to remove #comments
	CStringUtil::tokenize(line, KEY_COMMENT, listLineTokens);

	string lineParams = listLineTokens[0];

	// Remove unnessecary symbols from the param line
	CStringUtil::remove(LIST_REMOVE_SYM, lineParams);

	this->checkSection(lineParams);

	this->processParamLine(lineParams);

}

string BFileIniBase::get(const string& key) const {
	return this->m_row_params.getColumn(key);
}

string BFileIniBase::get(const string& section, const string& key) const {
	return this->get(this->doKey(section, key));
}

void BFileIniBase::dumpInfo() const {

	cout << __FILE__ << endl;

	this->m_row_params.dumpInfo();

}

/**
 * Check for a section and add it to the list of sections.
 */
void BFileIniBase::checkSection(const string& line) {

	string section;
	int posFound = 0;

	if (CStringUtil::findFirstMiddleSubstring(line, "[", "]", 0, section, &posFound)) {
		this->m_list_sections.push_back(section);
	}
}

/**
 * Check for Key=Value param on the line and add it to the key->value list.
 */
void BFileIniBase::processParamLine(const string& line) {

	std::vector<std::string> paramKeyVal;
	// Tokenize param line to get key=val
	CStringUtil::tokenize(line, KEY_PARAM_KEYVAL_SEPARATOR, paramKeyVal);

	if (paramKeyVal.size() == 2) {

		string key = paramKeyVal[0];

		// Add Section Key if any
		if (this->m_list_sections.size() > 0) {
			key = this->doKey(this->m_list_sections.back(), paramKeyVal[0]);
		}

		this->m_row_params.addColumn(key, paramKeyVal[1]);
	}

}

string BFileIniBase::doKey(const string& section, const string& key) const {
	return section + "::" + key;
}

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */


