/*
 * CPosIndexList.cc
 *
 *  Created on: 04.12.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "CPosIndexList.h"

using namespace std;

namespace SF {
namespace DB {

CPosIndexList::CPosIndexList() {
	// TODO Auto-generated constructor stub

}

CPosIndexList::~CPosIndexList() {
	// TODO Auto-generated destructor stub
}

void CPosIndexList::clear() {
	this->m_idx_list.clear();
	this->m_idx.clear();
}

size_t CPosIndexList::size() const {
	return this->m_idx_list.size();
}

/**
 * Add a new Index if NOT Existed.
 * If already exists do nothing.
 */
void CPosIndexList::add(const string& idx) {

	if (this->get(idx) >= 0) {

	} else {

		CPosIndex posIndex;

		this->m_idx.setIdx(idx, this->size());

		this->m_idx_list.push_back(posIndex);
	}
}

/**
 * Set a position index for a index by key
 */
void CPosIndexList::setIdx(const string& idx, const string& key,
		const size_t pos) {

	this->add(idx);

	size_t idxPos = this->get(idx);

	if (idxPos >= 0) {
		this->m_idx_list[idxPos].setIdx(key, pos);
	}

}

/**
 * Get a position for a Index from the list
 */
size_t CPosIndexList::get(const string& idx) {

	size_t pos = this->m_idx.getIdx(idx);

	if (pos >= size() || pos < 0) {
		pos = -1;
	}

	return pos;
}

/**
 * Get Pos Index from the list.
 */
size_t CPosIndexList::getIdx(const string& idx, const string& key) {

	size_t keyPos = -1;
	size_t idxPos = this->get(idx);

	if (idxPos >= 0) {
		keyPos = this->m_idx_list[idxPos].getIdx(key);
	}

	return keyPos;
}

} /* namespace DB */
} /* namespace SF */


