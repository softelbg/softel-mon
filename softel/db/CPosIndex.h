/*
 * CPosIndex.h
 *
 *	Position Index. The values are integer position indexes.
 *	Multiple index Keys could be used.
 *	This implementation uses single map and stores the keys into single std::string used as map key.
 *
 *  Created on: 27.06.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CPOSINDEX_H_
#define CPOSINDEX_H_

#include <map>
#include <vector>
#include <string>
#include <sstream>

#include <softel/core/intf/CListInterface.h>


namespace SF {
namespace DB {


class CPosIndex : public SF::CORE::INTF::CListInterface {

public:

	CPosIndex();
	~CPosIndex();

	bool setIdx(const std::string& key, size_t idx);
	bool setIdx(const std::vector<std::string>& key, size_t idx);
	bool setIdx(const std::string& key1, const std::string& key2, size_t idx);

	size_t getIdx(const std::string& key) const;
	size_t getIdx(const std::vector<std::string>& key) const;
	size_t getIdx(const std::string& key1, const std::string& key2) const;

	void clear();
	size_t size() const;

private:

	void doKey(const std::vector<std::string>& key, std::string& resultKey) const;

	std::map<std::string, size_t> m_index;
	std::map<std::string, size_t>::iterator m_it_index;


};

} /* namespace DB */
} /* namespace SF */

#endif /* CPOSINDEX_H_ */
