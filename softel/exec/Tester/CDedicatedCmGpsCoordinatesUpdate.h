/*
 * CDedicatedCmGpsCoordinatesUpdate.h
 *
 *  Created on: 21.12.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CDEDICATEDCMGPSCOORDINATESUPDATE_H_
#define CDEDICATEDCMGPSCOORDINATESUPDATE_H_

#include <string>

#include <softel/core/util/BFileReaderBase.h>
#include <softel/db/CDataSet.h>
#include <softel/db/CDataRow.h>

namespace SF {
namespace OSS {
namespace EXEC {

class CDedicatedCmGpsCoordinatesUpdate: public SF::CORE::UTIL::BFileReaderBase {
public:
	CDedicatedCmGpsCoordinatesUpdate();
	CDedicatedCmGpsCoordinatesUpdate(const std::string& fileName);
	virtual ~CDedicatedCmGpsCoordinatesUpdate();

	virtual void processLine(const std::string& line);

	void initCmList();

	void executeSql();

	void dumpInfo();

private:

	void prepareSql(const SF::DB::CDataRow& row);

	void setListPropertiesNames();
	void setListPropertiesNames(const std::string& line);

	std::vector<std::string> m_list_param_names;

	SF::DB::CDataSet m_list_cm;
	SF::DB::CDataSet m_list_sql;

};

} /* namespace EXEC */
} /* namespace OSS */
} /* namespace SF */

#endif /* CDEDICATEDCMGPSCOORDINATESUPDATE_H_ */
