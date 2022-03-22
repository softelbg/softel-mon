/*
 * CMapIndex.h
 *
 *  Created on: 13.03.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CMAPINDEX_H_
#define CMAPINDEX_H_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

template <typename T>
class CMapIndex {

public:

	CMapIndex();
	~CMapIndex();

	void clear();

	void addIndex(const string& idx);
	bool clearIndex(const string& idx);
	bool eraseIndex(const string& idx);
	int find(const string& idx, const T& key);

	bool addKeyVal(const string& idx, const T& key, const int val);

	void dumpInfo();

private:

	typename std::map<string, std::map<T, int> > m_map_idx;
	typename std::map<string, std::map<T, int> >::iterator m_it_idx;

	typename std::map<T, int>::iterator m_it_key_idx;
	typename std::map<T, int>::iterator m_it_current_key_idx;

};

template<typename T>
CMapIndex<T>::CMapIndex() {

}

template<typename T>
CMapIndex<T>::~CMapIndex() {

}

template<typename T>
void CMapIndex<T>::clear() {
	m_map_idx.clear();
}

/**
 * Add new Index (just an empty index if not existing)
 */
template<typename T>
void CMapIndex<T>::addIndex(const string& idx) {

	map<T, int> mapIndex;

	mapIndex.clear();

	m_it_idx = m_map_idx.find(idx);

	// Not existing, so add it
	if (m_it_idx == m_map_idx.end()) {

		m_map_idx[idx] = mapIndex;

	} else {
	// Already existing index...

	}

}

/**
 * Erase an Index
 */
template<typename T>
bool CMapIndex<T>::eraseIndex(const string& idx) {
	return (bool) m_map_idx.erase(idx);
}

/**
 * Clear all Index elements
 */
template<typename T>
bool CMapIndex<T>::clearIndex(const string& idx) {

	m_it_idx = m_map_idx.find(idx);

	// Idx Not found...
	if (m_it_idx == m_map_idx.end()) {
		return false;
	} else {
	// Idx Found, so clear key->val elements
		m_it_idx->second.clear();
	}

	return true;
}

/**
 * Find element for an index by its key
 */
template<typename T>
int CMapIndex<T>::find(const string& idx, const T& key) {

	int result = 0;

	m_it_idx = m_map_idx.find(idx);

	// Idx Not found...
	if (m_it_idx == m_map_idx.end()) {

		result = -1;

	} else {
		// Found index so find the key
		m_it_key_idx = m_it_idx->second.find(key);

		// Key Found
		if (m_it_key_idx != m_it_idx->second.end()) {

			result = m_it_key_idx->second;

		} else {

			result = -2;

		}

	}

	return result;
}

/**
 * Add new Key->Val element in index
 */
template<typename T>
bool CMapIndex<T>::addKeyVal(const string& idx, const T& key, const int val) {

	m_it_idx = m_map_idx.find(idx);

	// Idx Not found, so first add it.
	if (m_it_idx == m_map_idx.end()) {

		addIndex(idx);

		m_it_idx = m_map_idx.find(idx);

		// If after add is not found, something is wrong (should never happen...)
		if (m_it_idx == m_map_idx.end()) {
			return false;
		}
	}

	// TODO: Consider what to do if the element already existed...
	m_it_idx->second[key] = val;

	return true;
}

template<typename T>
void CMapIndex<T>::dumpInfo() {

	cout << __FILE__ << " -------------------------- index count:" << m_map_idx.size() << endl;

	int i;
	for(i = 0, m_it_idx = m_map_idx.begin(); m_it_idx != m_map_idx.end(); m_it_idx++, i++) {
		//cout << "idx[" << i << "] " << m_it_idx->first << " => " << m_it_idx->second << endl;
	}

	cout << endl;

}


#endif /* CMAPINDEX_H_ */
