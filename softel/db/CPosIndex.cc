/*
 * CPosIndex.cc
 *
 *  Created on: 27.06.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CPosIndex.h"

using namespace std;

namespace SF {
namespace DB {

CPosIndex::CPosIndex() {

}

CPosIndex::~CPosIndex() {

}

/**
 * Concatenate all listed Keys into a big result Key.
 */
void CPosIndex::doKey(const vector<string>& listKey, string& resultKey) const {
	stringstream ssKey;

	for (size_t i = 0; i < listKey.size(); i++) {
		ssKey << "k" << i << "_" << listKey[i];
	}

	resultKey = ssKey.str();

}

/**
 * Add a multiple keys index.
 * Keys should be concatenated in some manner into a string.
 */
bool CPosIndex::setIdx(const vector<string>& listKey, size_t idx) {

	string resultKey;

	doKey(listKey, resultKey);

	m_index[resultKey] = idx;

	return true;
}

/**
 * Add a single key index
 */
bool CPosIndex::setIdx(const string& key, size_t idx) {

	vector<string> listKey;

	listKey.push_back(key);

	return setIdx(listKey, idx);
}

/**
 * Add 2 keys index. Suppose it will be most used multiple keys index.
 *
 */
bool CPosIndex::setIdx(const string& key1, const string& key2, size_t idx) {

	vector<string> listKey;

	listKey.push_back(key1);
	listKey.push_back(key2);

	return setIdx(listKey, idx);

}

/**
 * Get a single key index
 */
size_t CPosIndex::getIdx(const string& key) const {

	vector<string> listKey;

	listKey.push_back(key);

	return getIdx(listKey);
}

/**
 * Get a multiple key index.
 */
size_t CPosIndex::getIdx(const vector<string>& listKey) const {

	string resultKey;

	doKey(listKey, resultKey);

	map<string, size_t>::const_iterator it;

	it = m_index.find(resultKey);

	if (it != m_index.end()) {
		return it->second;
	} else {
		return -1;
	}

}

/**
 * Get 2 keys index.
 */
size_t CPosIndex::getIdx(const string& key1, const string& key2) const {

	vector<string> listKey;

	listKey.push_back(key1);
	listKey.push_back(key2);

	return getIdx(listKey);
}


void CPosIndex::clear() {
	m_index.clear();
}

size_t CPosIndex::size() const {
	return m_index.size();
}


} /* namespace DB */
} /* namespace SF */


