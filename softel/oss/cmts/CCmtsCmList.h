/*
 * CCmtsCmList.h
 *
 *	CMTS CM List representation.
 *	Should contain a vector of CmtsCm and Position multikeyed Indexing.
 *
 *  Created on: 18.07.2012
 *      Author: STAN
 *
 *   	www.softel.bg
 *
 *   	Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCMTSCMLIST_H_
#define CCMTSCMLIST_H_

#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include <unistd.h>

#include "CCmts.h"
#include "CCmtsCm.h"
#include "CBaseList.h"
#include <softel/db/CPosIndex.h>

namespace SF {
namespace OSS {
namespace CMTS {

class CCmtsCmList : public CBaseList {

public:

	CCmtsCmList();
	~CCmtsCmList();

	// TODO: Add a addCm with multiple indexes. Now only indexed by mac, sid
	void addCm(CCmtsCm* cm);

	CCmtsCm* getCm(int pos);
	CCmtsCm* getCm(const std::string& key1, const std::string& key2);

	vector<CCmtsCm*>& getCmList();

	void clear();
	int size();

	void dumpInfo();

private:

	std::vector<CCmtsCm*> m_cm_list;
	SF::DB::CPosIndex idx;

};


} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */


#endif /* CCMTSCMLIST_H_ */
