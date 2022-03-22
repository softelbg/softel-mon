/*
 * CCmtsIfRawCmCounter.h
 *
 *	Count bad CMs for each CMTS Interface
 *
 *  Created on: 27.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CCMTSIFRAWCMCOUNTER_H_
#define CCMTSIFRAWCMCOUNTER_H_

#include <vector>
#include <map>
#include <string>

#include <softel/core/intf/CDumpableInterface.h>
#include <softel/core/intf/CListInterface.h>
#include <softel/db/CDataSet.h>
#include <softel/db/CDataRow.h>
#include "CSqlAlertTools.h"

namespace SF {
namespace OSS {
namespace ALERT {

class CCmtsIfRawCmCounter :
		public SF::CORE::INTF::CListInterface,
		public SF::CORE::INTF::CDumpableInterface {
public:
	CCmtsIfRawCmCounter();
	virtual ~CCmtsIfRawCmCounter();

	void init();

	virtual void clear();
	virtual size_t size() const;
	virtual size_t size(const std::string& cmts_id) const;

	void add(const SF::DB::CDataRow& rowCmRaw);

	const std::map<std::string, std::map<std::string, int> >& getRawCmCount() const;
	const std::map<std::string, int>& getRawCmCount(const std::string& cmts_id) const;
	const int getRawCmCount(const std::string& cmts_id, const std::string& if_id) const;

	void dumpInfo();

private:

	void increment(const std::string& cmts_id, const std::string& if_id);
	void increment(const std::string& cmts_id, const std::vector<std::string>& if_id_list);
	void increment(const std::string& cmts_id, const std::map<std::string, std::string>& if_id_list);

	SF::DB::CDataSet m_docs3_interfaces; // Should contain docs3 cmts if -> CMs list and indexed by CM
	std::multimap<std::string, std::string> m_cm_if_list;

	std::map<std::string, std::map<std::string, int> > m_count_cm; // TODO: Consider specific implementation...

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CCMTSIFRAWCMCOUNTER_H_ */
