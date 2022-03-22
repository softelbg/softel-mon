/*
 * CThresholdBase.h
 *
 *	Base Threshold. Should represent a single threshold.
 *
 *  Created on: 02.10.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef CTHRESHOLDBASE_H_
#define CTHRESHOLDBASE_H_

#include "CSqlThresholdTools.h"
#include "CommonAlert.h"

#include <softel/db/CParamSet.h>
#include <softel/db/CDataSet.h>
#include <softel/db/CDataRow.h>

namespace SF {
namespace OSS {
namespace ALERT {

class CThresholdBase {
public:
	CThresholdBase();
	CThresholdBase(const SF::DB::CDataRow& dr);
	virtual ~CThresholdBase();

	virtual void init();
	virtual void init(const SF::DB::CDataRow& dr);

	std::string getParam(const std::string& key);
	float getValue();
	int getSeverity();
	std::string getColor();
	std::string getCondition();

	bool compare(const std::string& value);
	bool compare(float value);

	virtual void dumpInfo() const;

protected:

	SF::DB::CDataRow m_data_row;
	SF::DB::CParamSet m_param_set;

};

} /* namespace ALERT */
} /* namespace OSS */
} /* namespace SF */

#endif /* CTHRESHOLDBASE_H_ */
