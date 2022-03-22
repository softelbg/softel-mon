/*
 * CDedicatedCmList.h
 *
 *	Test/Tool for initialize Dedicated CM List from a file into DB.
 *
 *  Created on: 18.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CDEDICATEDCMLIST_H_
#define CDEDICATEDCMLIST_H_

#include <softel/core/util/BFileReaderBase.h>

#include <string>
#include <vector>

namespace SF {
namespace OSS {
namespace EXEC {

class CDedicatedCmList : public SF::CORE::UTIL::BFileReaderBase {
public:
	CDedicatedCmList();
	CDedicatedCmList(const std::string& fileName);
	virtual ~CDedicatedCmList();

	virtual void processLine(const std::string& line);

	void executeSqlList();

	void dumpInfo();

private:

	void prepareSql(const std::vector<std::string>& listParams, std::string& sqlResult);

	std::vector<std::string>	m_list_sql;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CDEDICATEDCMLIST_H_ */
