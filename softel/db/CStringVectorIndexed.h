/*
 * CStringVectorIndexed.h
 *
 *	Indexed String Vector. Using CPosIndex for multiple key indexing.
 *
 *  Created on: 05.11.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CSTRINGVECTORINDEXED_H_
#define CSTRINGVECTORINDEXED_H_

#include <vector>

#include <softel/core/intf/CListInterface.h>
#include "CPosIndex.h"

namespace SF {
namespace DB {

class CStringVectorIndexed : public SF::CORE::INTF::CListInterface {
public:
	CStringVectorIndexed();
	virtual ~CStringVectorIndexed();

	void add(const std::string& value);
	void add(const std::string& key, const std::string& value);
	void add(const std::vector<std::string>& listKeys, const std::string& value);

	std::string get(size_t pos);
	bool get(size_t pos, std::string& result);

	std::string get(const std::string& key);
	bool get(const std::string& key, std::string& result);
	bool get(const std::string& key1, const std::string& key2, std::string& result);
	bool get(const std::vector<std::string>& listKeys, std::string& result);

	void clear();
	size_t size() const;

private:

	std::vector<std::string>	m_list;
	CPosIndex		m_index;
};

} /* namespace DB */
} /* namespace SF */

#endif /* CSTRINGVECTORINDEXED_H_ */
