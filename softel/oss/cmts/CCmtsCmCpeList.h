/*
 * CCmtsCmCpeList.h
 *
 *  Created on: 22.04.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCMTSCMCPELIST_H_
#define CCMTSCMCPELIST_H_

#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include "CCpe.h"

namespace SF {
namespace OSS {
namespace CMTS {

class CCmtsCmCpeList {

public:

	CCmtsCmCpeList();
	~CCmtsCmCpeList();

	void clear();
	int size();

	void addCpe(const std::string& ip, const std::string& mac);
	void addCpe(const CCpe& cpe);
	CCpe& getCpe(const int pos);

private:

	std::vector<CCpe> m_cpe_list;

};


} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CCMTSCMCPELIST_H_ */
