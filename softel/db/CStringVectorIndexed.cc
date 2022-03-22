/*
 * CStringVectorIndexed.cc
 *
 *  Created on: 05.11.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CStringVectorIndexed.h"

using namespace std;

namespace SF {
namespace DB {

CStringVectorIndexed::CStringVectorIndexed() {
	this->clear();
}

CStringVectorIndexed::~CStringVectorIndexed() {
	this->clear();
}

void CStringVectorIndexed::add(const string& value) {
	this->m_list.push_back(value);
}

void CStringVectorIndexed::add(const string& key, const string& value) {
	this->m_index.setIdx(key, this->size());
	this->add(value);
}

void CStringVectorIndexed::add(const vector<string>& listKeys,
		const string& value) {
	this->m_index.setIdx(listKeys, this->size());
	this->add(value);
}

string CStringVectorIndexed::get(size_t pos) {

	string result;

	this->get(pos, result);

	return result;
}

string CStringVectorIndexed::get(const string& key) {

	string result;

	this->get(key, result);

	return result;
}

bool CStringVectorIndexed::get(size_t pos, string& result) {

	if (pos >= 0 && pos < this->size()) {

		result = this->m_list[pos];

		return true;
	} else {

		result.clear();

		return false;
	}

}

bool CStringVectorIndexed::get(const string& key, string& result) {

	result.clear();

	size_t pos = this->m_index.getIdx(key);

	return this->get(pos, result);
}

bool CStringVectorIndexed::get(const string& key1, const string& key2,
		string& result) {

	result.clear();

	size_t pos = this->m_index.getIdx(key1, key2);

	return this->get(pos, result);
}

bool CStringVectorIndexed::get(const vector<string>& listKeys, string& result) {

	result.clear();

	size_t pos = this->m_index.getIdx(listKeys);

	return this->get(pos, result);
}

void CStringVectorIndexed::clear() {
	this->m_list.clear();
	this->m_index.clear();
}

size_t CStringVectorIndexed::size() const {
	return this->m_list.size();
}

} /* namespace DB */
} /* namespace SF */


