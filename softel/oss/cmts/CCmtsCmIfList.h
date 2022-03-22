/*
 * CCmtsCmIfList.h
 *
 *  Created on: 27.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCMTSCMIFLIST_H_
#define CCMTSCMIFLIST_H_

#include <map>
#include <string>

#include "CCmtsCm.h"
#include "CCmtsIf.h"
#include "SqlTableNames.h"

#include <softel/db/CParamSet.h>
#include <softel/db/CPosIndex.h>
#include <softel/core/intf/CListInterface.h>


namespace SF {
namespace OSS {
namespace CMTS {

class CCmtsCmIfList: public SF::CORE::INTF::CListInterface {
public:
	CCmtsCmIfList();
	CCmtsCmIfList(const std::string& cmts_id);
	virtual ~CCmtsCmIfList();

	void clear();
	size_t size() const;

	int sizeList(const std::string& key, const multimap<std::string, std::string>& source);
	int sizeCm(const std::string& if_id);
	int sizeIf(const std::string& cm_mac);

	void setCmtsId(const std::string& cmts_id);
	std::string getCmtsId();

	void add(const std::string& cm_mac, const std::string& if_id);
	void add(const CCmtsCm& cm, const std::string& if_id);

	void getList(const std::string& key, const multimap<std::string, std::string>& source,
			vector<std::string>& dest_list); // TODO: Place in a Base class

	void getCmList(const std::string& if_id, vector<std::string>& cm_id_list);
	void getIfList(const std::string& cm_mac, vector<std::string>& if_id_list);

	int find(const std::string& cm_mac, const std::string& if_id);
	int find(CCmtsCm* pCm, CCmtsIf* pIf);

private:

	SF::DB::CParamSet m_param_set;

	std::multimap<std::string, std::string> m_list_cm;
	std::multimap<std::string, std::string> m_list_if;

	SF::DB::CPosIndex m_idx_cmifpair;

};

} /* namespace CMTS */
} /* namespace OSS */
} /* namespace SF */

#endif /* CCMTSCMIFLIST_H_ */
