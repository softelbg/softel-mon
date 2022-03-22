/*
 * CPosIndexList.h
 *
 *	List of Position Indexes. Should contain a vector of CPosIndexes.
 *	Each pos index is indexed in the list also.
 *	So searching for a position should use index for the index and key for the position.
 *
 *  Created on: 04.12.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CPOSINDEXLIST_H_
#define CPOSINDEXLIST_H_

#include <vector>
#include <string>

#include <softel/core/intf/CListInterface.h>
#include "CPosIndex.h"


namespace SF {
namespace DB {


class CPosIndexList: public SF::CORE::INTF::CListInterface {
public:
	CPosIndexList();
	virtual ~CPosIndexList();

	void clear();
	size_t size() const;

	void add(const std::string& idx);
	size_t get(const std::string& idx);

	void setIdx(const std::string& idx, const std::string& key, const size_t pos);
	size_t getIdx(const std::string& idx, const std::string& key);

private:

	std::vector<CPosIndex> m_idx_list;

	CPosIndex m_idx;

};

} /* namespace DB */
} /* namespace SF */

#endif /* CPOSINDEXLIST_H_ */
