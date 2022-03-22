/*
 * CIndexedVector.h
 *
 *  Created on: 13.03.2012
 *      Author: sgeorgiev
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CINDEXEDVECTOR_H_
#define CINDEXEDVECTOR_H_

#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include "CMapIndex.h"

namespace SF {
namespace DB {

template<typename T, typename U>
class CIndexedVector {

public:

	CIndexedVector();
	~CIndexedVector();

	void clear();
	size_t size();

	void add(const T& element, const std::string& idx, const U& key);

	T* get(const int pos);
	T* get(const std::string& idx, const U& key);

	// Indexes
	bool addIndex(const std::string& idx);
	//bool buildIndex(const std::string& idx);

	void dumpInfo();

private:

	std::vector<T> m_vector;

	CMapIndex<U> m_index;

};

template<typename T, typename U>
CIndexedVector<T, U>::CIndexedVector() {
	clear();
}

template<typename T, typename U>
CIndexedVector<T, U>::~CIndexedVector() {

}

template<typename T, typename U>
void CIndexedVector<T, U>::clear() {

}

template<typename T, typename U>
size_t CIndexedVector<T, U>::size() {
	return m_vector.size();
}

/**
 * Add an Element into std::vector and make index record for it (index name and element key)
 */
template<typename T, typename U>
void CIndexedVector<T, U>::add(const T& element, const std::string& idx,
		const U& key) {

	size_t pos = size();

	m_index.addKeyVal(idx, key, pos);

	m_vector.push_back(element);
}

/**
 * Get an Element by std::vector position
 */
template<typename T, typename U>
T* CIndexedVector<T, U>::get(const int pos) {

	if (pos < 0 || pos >= size()) {
		return NULL;
	} else {
		return &(m_vector[pos]);
	}

}

/**
 * Get an Element by index name and element key
 */
template<typename T, typename U>
T* CIndexedVector<T, U>::get(const std::string& idx, const U& key) {

	int pos = m_index.find(idx, key);

	if (pos < 0 || pos >= size()) {
		return NULL;
	} else {
		return &(m_vector[pos]);
	}

}

template<typename T, typename U>
bool CIndexedVector<T, U>::addIndex(const std::string& idx) {

	m_index.addIndex(idx);

	return true;
}

/*
 template <typename T, typename U>
 bool CIndexedVector<T, U>::buildIndex(const std::string& idx) {

 m_index.addIndex(idx);

 m_index.clearIndex(idx);

 for(unsigned int i = 0; i < size(); i++) {

 }

 }
 */

template<typename T, typename U>
void CIndexedVector<T, U>::dumpInfo() {

	cout << __FILE__ << " -------------------------- size:" << size() << endl;

	m_index.dumpInfo();

}

} /* namespace DB */
} /* namespace SF */

#endif /* CINDEXEDVECTOR_H_ */
