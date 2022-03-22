/*
 * CParamSet.h
 *
 *	Simple String Parameter Set of key->value
 *
 *  Created on: 01.08.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CPARAMSET_H_
#define CPARAMSET_H_

#include <stdio.h>
#include <string>
#include <map>
#include <vector>

#include <softel/core/CObject.h>
#include <softel/core/intf/CListInterface.h>


namespace SF {
namespace DB {


class CParamSet : public SF::CORE::CObject, public SF::CORE::INTF::CListInterface {

public:
	CParamSet();
	~CParamSet();

	void clear();
	size_t size() const;

	void setParam(const std::string& key, const std::string& val);
	void setParam(const std::string& key, int val);
	void setParam(const std::string& key, long val);

	std::string getParam(const std::string& key) const;
	bool getParam(const std::string& key, std::string& val) const;

	bool hasParam(const std::string& key) const;

	void getKeyValueList(std::vector<std::string>& keys, std::vector<std::string>& values) const;

	void dumpInfo() const;
	std::string toString() const;

protected:
	std::map<std::string, std::string> m_param_set;
};

} /* namespace DB */
} /* namespace SF */

#endif /* CPARAMSET_H_ */
